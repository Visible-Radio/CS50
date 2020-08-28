
SELECT title, year FROM movies WHERE id IN

-- You can select from another select statement without going through a column
-- SELECT all the movie ids FROM (A list of movies with Johnny Depp)
-- WHERE those movie ids also are IN (A list of movies with Helena Bonham Carter)
-- No AND opperator required
(
SELECT movie_id FROM (SELECT movie_id FROM stars WHERE  person_id IN (SELECT id FROM people WHERE name ="Helena Bonham Carter"))

WHERE movie_id IN (SELECT movie_id FROM stars WHERE person_id IN (SELECT id FROM people WHERE name="Johnny Depp"))
)

;



