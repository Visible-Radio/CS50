SELECT name FROM people WHERE name != "Kevin Bacon" AND id IN
(
	SELECT person_id FROM stars WHERE movie_id IN
		(
		-- Get the movie_ids for all movies Kevin Bacon was in
		SELECT movie_id FROM stars WHERE person_id=(SELECT id FROM people WHERE birth ="1958" AND name="Kevin Bacon")
		)
)

;

