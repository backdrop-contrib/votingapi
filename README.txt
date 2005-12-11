The Basics
==========
A generalized voting API for Drupal. It does not provide any voting functionality per se, but exposes a standardized API for casting and tabulating user votes for content. In many cases, different voting and rating modules can even share data. While it's not possible in all circumstances, the basic infrastructure makes it more feasible.

Most modules will use three basic functions:

votingapi_set_vote($content_type, $content_id, $vote, $uid) -- This casts a single vote for a piece of content
votingapi_get_user_votes($content_type, $content_id, $uid) -- This pulls the votes a user has cast for a piece of content
votingapi_get_voting_results($content_type, $content_id) -- This pulls the aggregate of ALL votes cast for a piece of content

In all of these examples, $content_type and $content_id are used to identify the content being rated. 'node', 'user,' 'comment,' and 'aggregator_item' are possible $content_types, while $content_id is just the record's primary key.

Under The Hood
==============
The $vote parameter for votingapi_set_vote() deserves special attention.
  1: a number. This is assumed to be a 0-100 percentage value, and is inserted with default value_type and tag.
  2: an object, with $vote->value, $vote->value_type, and $vote->tag properties.
  3: an array of $vote objects as used in #2. This allows efficient casting of multi-criteria votes.

The $value_type and $tag properties, while optional, allow a lot of flexibility.

value_type
  An optional int representing the meaning of the value param. Three standard types are defined by votingapi:
  VOTINGAPI_VALUE_TYPE_PERCENT -- 'Value' is a number from 0-100. The API will cache an average for all votes. 
  VOTINGAPI_VALUE_TYPE_TOKEN   -- 'Value' is a positive or negative int. The API will cache the sum of all votes.
  VOTINGAPI_VALUE_TYPE_KEY     -- 'Value' is a foreign key. The API will cache a vote-count for each discrete value.
  Other value_types can be passed in, but no default actions will be taken with them by the API. If no value is passed in, VOTINGAPI_VALUE_TYPE_PERCENT is the default.
tag
  A string to separate multiple voting criteria. For example, a voting system that rates software for 'stability' and 'features' would cast two votes, each with a different tag. If none is specified, the default 'vote' tag is used.

When the voting api system calculates aggregate results, it's smart enough to separate them out into tag and value_type specific ones. Modules that want to add aggregate calculations or other niceness can implement the module_votingapi() hook's "calculate" $op. It gives modules a chance to loop through the raw votes and add additional aggregate functions like 'highest' or 'median' before the totals for a given piece of content are cached. _votingapi_recalculate_results() has some examples of how this can be done.

Slicing And Dicing
==================
Whenever a vote is saved, the votingapi_cache table is updated with aggregate data for the content that's being rated. That table is where you'll find things like averages, etc. If you want to create ranked or filtered queries (top-rated nodes, ordered by average vote, for example) it's the table to use.


WARNING
=======
If you're a module developer who's been developing based on a version of votingapi earlier than 1.5, take heed! Versions 1.5 and later user a different API and a different table structure. There's a lot less work for you to do, but it will require some changes. Older versions are still obtainable in CVS if you need them for compatability reasons.