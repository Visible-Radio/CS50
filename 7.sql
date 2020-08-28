SELECT title, rating FROM
movies JOIN ratings ON movies.id=ratings.movie_id
WHERE movies.year=2010 and ratings.rating != "\\N"
ORDER BY rating DESC, title ASC;