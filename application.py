import os

from cs50 import SQL
from flask import Flask, flash, jsonify, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Ensure responses aren't cached
@app.after_request
def after_request(response):
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""

    # SQL querry to get all of the user's holdings
    portfolio=db.execute("SELECT shares, symbol FROM holdings WHERE client_id=:id", id=session['user_id'])
    print(portfolio)
    cash=db.execute("SELECT cash FROM users WHERE id=:id", id=session['user_id'])
    print(f"User cash holdings {usd(cash[0]['cash'])}")

    # portfolio passes out the symbol and the number of shares held
    # for each row we need to also lookup the name and price of the stock and pass those out
    # also do some math and pass out for each holding the total value shares * price
    # can we insert a new key value pair into each dict in portfolio???
    Ttotal=0
    for row in portfolio:
        lookup_results=(lookup(row['symbol']))
        row['name']=lookup_results['name']
        row['price']=usd(lookup_results['price'])
        row['total']=usd(float(lookup_results['price'])*int(row['shares']))
        Ttotal += float(lookup_results['price'])*int(row['shares'])


    return render_template("index.html", portfolio=portfolio, cash=usd(cash[0]['cash']), Ttotal=usd(Ttotal+cash[0]['cash']))


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "POST":
        if not request.form.get("symbol"):
            return apology("Please specify a stock to purchase.", 403)

        elif (not request.form.get("shares")) or (not request.form.get("shares").isdecimal()) \
        or int(request.form.get("shares")) <1:
            return apology("Please input number of shares to purchase.", 403)

        else:
            # querry API
            lookup_results=lookup(request.form.get("symbol"))

            # make sure return is not Null
            if lookup_results == None:
                return apology("Invalid Symbol", 400)
            else:
                # print the returned data from lookup to terminal to see what it is
                print(lookup_results)
                print(lookup_results['symbol'])


                # check available funds for user that is in session
                cash=db.execute("SELECT cash FROM users WHERE id = :id", id=session['user_id'])
                print(f"Session user: {session['user_id']}")
                print(f"Funds available: {usd(cash[0]['cash'])}")

                # compare the price of the stock to the available funds
                cost=lookup_results['price']*float(request.form.get('shares'))
                print(f"Cost: {usd(cost)}")

                if cost > cash[0]['cash']:
                    return apology("Insufficient funds")
                else:
                    # deduct cost from cash for session user
                    # update cash for session user
                    new_cash = cash[0]['cash']-cost
                    print(f"Cash after transaction: {usd(new_cash)}")
                    db.execute("UPDATE users SET cash=:new_cash WHERE id=:id", new_cash=new_cash, id=session['user_id'])
                    # add transaction to transactions table

                    db.execute("INSERT INTO transactions (client_id, symbol, type, shares_moved, t_price) \
                    VALUES (:client_id, :symbol, :type, :shares_moved, :t_price)", \
                    client_id=session['user_id'], symbol=lookup_results['symbol'], type="Buy", \
                    shares_moved=request.form.get('shares'), t_price=lookup_results['price'])

                    # update holdings table
                    # client_id, symbol, shares

                    #check if user has an entry in the holdings table where the symbol just being bought appears
                    #if yes, UPDATE this entry by adding it's current value with the shares just purchased
                    #if no, INSERT a new entry with the client_id, the shares purchased and the symbol

                    holdings_return = db.execute("SELECT * FROM holdings WHERE client_id = :client_id", client_id=session['user_id'])
                    print(holdings_return)

                    if len(holdings_return) == 0:
                        db.execute("INSERT INTO holdings (client_id, shares, symbol) \
                            VALUES (:client_id, :shares, :symbol)", client_id = session['user_id'], \
                            shares=request.form.get('shares'), symbol=lookup_results['symbol'])
                        print(holdings_return)

                    else:
                        found=False
                        for record in holdings_return:
                             if record['symbol'] == lookup_results['symbol']:
                                 print("holdings_return list of dicts for this user contains entry for this stock")
                                 new_shares = int(request.form.get('shares')) + int(record['shares'])
                                 db.execute("UPDATE holdings SET shares = :shares WHERE (client_id = :id AND symbol=:symbol)", \
                                 shares=new_shares,id=session['user_id'], symbol=lookup_results['symbol'])
                                 found=True
                                 break
                        if found==False:
                            db.execute("INSERT INTO holdings (client_id, shares, symbol) \
                            VALUES (:client_id, :shares, :symbol)", client_id = session['user_id'], \
                            shares=request.form.get('shares'), symbol=lookup_results['symbol'])
                            print(holdings_return)
                    flash("Bought!")
                    return redirect("/")

    else:
        #display the page
        return render_template("buy.html")

@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    history=db.execute("SELECT symbol, shares_moved, t_price, t_time, type FROM transactions WHERE client_id=:id", \
    id=session['user_id'])
    print(history)
    for row in history:
        row['t_price']=usd(row['t_price'])


    return render_template("history.html", history=history)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username",
                          username=request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    if request.method == "POST":
        lookup_results=lookup(request.form.get("symbol"))

        # print the returned data from lookup to terminal to see what it is
        print(lookup_results)
        if lookup_results == None:
            return apology("Invalid Symbol", 400)
        else:
            return render_template("quoted.html", **lookup_results, fprice=usd(lookup_results['price']))
    else:
        #display the page
        return render_template("quote.html")



@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    # if method is GET, just display the registration page
    # if the method is POST, do some other stuff
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Ensure confirmation was submitted
        elif not request.form.get("confirmation"):
            return apology("must confirm password", 403)

        # Check if password and confirmation match
        elif request.form.get("confirmation") != request.form.get("password"):
            return apology("Password and confirmation must match")


        # check if username is available - this feels like a dirty way of doing
        checkname = db.execute("SELECT username FROM users WHERE username = :username", username=request.form.get("username"))
        # print the list of dicts returned by db.execute to the terminal
        # it should return a list containing one dict containing one key value pair
        print(checkname)
        if len(checkname) != 0:
            return apology("Username not available")
        else:
            new_username = request.form.get("username")
            new_password_confirm = request.form.get("confirmation")
            new_password = request.form.get("password")
            hash = generate_password_hash(new_password)

            db.execute("INSERT INTO users (username, hash) VALUES (:username, :hash)", username=new_username, hash=hash)

            # placeholder return statement.  CS50 staff solution logs the newly created user in
            # and takes them to a portfolio summary page with an alert at the top that says "registered"
            # need some js to do this probably?
            return render_template("login.html")
    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    # get the user's current holdings to populate the <select> menu
    portfolio=db.execute("SELECT symbol, shares FROM holdings WHERE client_id=:id", id=session['user_id'])
    print(f"User's current holdings {portfolio}")
    cash=db.execute("SELECT cash FROM users WHERE id=:id", id=session['user_id'])
    cash=float(cash[0]['cash'])
    print(f"user's cash holdings prior to sale: {usd(cash)}")

    if request.method == "POST":
        print(request.form.get("symbol"))
        if request.form.get("symbol")==None:
            return apology("You must select a stock to sell")

        for row in portfolio:
            if row['symbol']==request.form.get("symbol"):
                if int(request.form.get("shares")) > int(row['shares']):
                    return apology("You cannot sell more shares than you hold")
                else:
                    # we've found the dict containing the desired symbol
                    # and the user holds at least as many shares of the stock as they intend to sell
                    # lookup the current price for the stock
                    lookup_results=lookup(row['symbol'])
                    print(f"Lookup results {lookup_results}")
                    print(f"price at transaction for {row['symbol']} is {lookup_results['price']}")

                    # credit this amount to user's cash
                    #lookup_results['price']*request.form.get("shares")

                    db.execute("UPDATE users SET cash=:new_cash WHERE id=:id", \
                    new_cash=cash+(float(lookup_results['price'])*float(request.form.get("shares"))), \
                    id=session['user_id'])

                    # deduct this amount from user's holding for the stock
                    #request.form.get("shares")

                    db.execute("UPDATE holdings SET shares=:new_shares WHERE (symbol=:symbol AND client_id=:id)", \
                    new_shares=float(row['shares'])-float(request.form.get("shares")), \
                    symbol=row['symbol'], id=session['user_id'])

                    db.execute("INSERT INTO transactions (client_id, symbol, type, shares_moved, t_price) \
                    VALUES (:client_id, :symbol, :type, :shares_moved, :t_price)", \
                    client_id=session['user_id'], symbol=row['symbol'], type="Sell", \
                    shares_moved=request.form.get('shares'), t_price=lookup_results['price'])


                    flash("Sold!")
                    #return render_template("sell.html", portfolio=portfolio)
                    return redirect("/")

    else:
        #display the page
        return render_template("sell.html", portfolio=portfolio)


def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
