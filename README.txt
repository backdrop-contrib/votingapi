A generalized voting API for Drupal. Votes are stored with a four-value key: content_type, content_id, tag, and uid. While modules can use these fields for anything if  authors really feel like it, it's recommended that the following standards be followed:

content_type:  'node', 'comment', 'user', 'aggregator-item', 'link', etc.
content_id:    the primary key/id of the record
tag:           in multi-criteria voting (for example'graphics' and 'sound'
               for games), the specific criteria is stored in the 'tag' column 

Modules interested in storing and retrieving aggregate data (median votes, or highest vote) can implement their own select functions, or call votingapi_get_votes() and loop through the $votes->raw_votes array.

Modules that want to add aggregate calculations or other niceness for the benefit of other modules can implement the module_votingapi() hook's "calculate" $op. It gives modules a chance to loop through the raw votes and add additional aggregate properties like 'highest vote' or 'median vote' to the $votes collection before it's passed on to calling modules.