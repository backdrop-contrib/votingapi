<?php

function votingapi_views_tables() {
  $tables["votingapi_cache"] = array(
    "name" => "votingapi_cache", 
    "provider" => "votingapi",
    "join" => array(
      "left" => array(
        "table" => "node",
        "field" => "nid"
      ), 
      "right" => array(
        "field" => "content_id"
      ),
      "extra" => array(
        'content_type' => 'node',
      ),
    ),
    "fields" => array(
      "value" => array(
        'name' => t("VotingAPI: Voting results"),
        'sortable' => true,
        'addlfields' => array('value_type', 'tag', 'function'),
      ),
    ),
    "sorts" => array(
      "value" => array('name' => "Voting result")
    ),
    "filters" => array(
      "value_type" => array(
        'name' => "VotingAPI: Result type",
        'operator' => array("=" => "Is", "!=" => "Is Not"),
        'list' => array(
          VOTINGAPI_VALUE_TYPE_PERCENT => 'Percentage',
          VOTINGAPI_VALUE_TYPE_TOKEN => 'Points',
          VOTINGAPI_VALUE_TYPE_KEY => 'Custom',
        ),
        'list-type' => "select",
        'help' => "Filter by the type of result being calculated (Percentage, points, etc)",
      ),
      "criteria" => array(
        'name' => "VotingAPI: Result criteria",
        'operator' => array("=" => "Is", "!=" => "Is Not"),
        'list' => "votingapi_handler_cache_vote_tags",
        'list-type' => "select",
        'help' => "Filter by the criteria being voted on",
      ),
      "function" => array(
        'name' => "VotingAPI: Result function",
        'operator' => array("=" => "Is", "!=" => "Is Not"),
        'list' => "votingapi_handler_cache_vote_functions",
        'list-type' => "select",
        'help' => "Filter by the function used to aggregate results (total number of votes, average vote, etc)",
      ),
    ),
  );
  
  $tables["votingapi_vote"] = array(
    "name" => "votingapi_vote", 
    "provider" => "votingapi",
    "join" => array(
      "left" => array(
        "table" => "node",
        "field" => "nid"
      ), 
      "right" => array(
        "field" => "content_id"
      ),
      "extra" => array(
        'content_type' => 'node',
      ),
    ),
    "fields" => array(
      "value" => array(
        'name' => t("VotingAPI: Individual vote"),
        'sortable' => true,
        'addlfields' => array('value_type', 'tag', 'function', 'uid'),
      ),
    ),
    "sorts" => array(
      "value" => array('name' => "Vote value")
    ),
    "filters" => array(
      "value_type" => array(
        'name' => "VotingAPI: Vote type",
        'operator' => array("=" => "Is", "!=" => "Is Not"),
        'list' => array(
          VOTINGAPI_VALUE_TYPE_PERCENT => 'Percentage',
          VOTINGAPI_VALUE_TYPE_TOKEN => 'Points',
          VOTINGAPI_VALUE_TYPE_KEY => 'Custom',
        ),
        'list-type' => "select",
        'help' => "Filter by the type of result being calculated (Percentage, points, etc)",
      ),
      "criteria" => array(
        'name' => "VotingAPI: Vote criteria",
        'operator' => array("=" => "Is", "!=" => "Is Not"),
        'list' => "votingapi_handler_vote_tags",
        'list-type' => "select",
        'help' => "Filter by the criteria being voted on",
      ),
      "currentuidtouched" => array(
        'field' => 'uid',
        'name' => "VotingAPI: Current user has voted",
        'operator' => array('=' => "voted on by"),
        'list' => "views_handler_filter_usercurrent",
        'list-type' => 'select',
        'handler' => "votingapi_views_handler_filter_uid_voted",
        'help' => "This allows you to filter by whether or not the logged in user has voted on the node.",
      ),
    ),
  );

  return $tables;
}

function votingapi_handler_cache_vote_functions() {
  static $votingapi_cache_functions;
  if (!is_array($votingapi_cache_functions)) {
    $votingapi_cache_functions = array();
    $votingapi_cache_functions = array(
      'sum' => 'Sum of vote values',
      'count' => 'Number of votes',
      'average' => 'Average vote value',
    );

    $result = db_query('SELECT DISTINCT function FROM {votingapi_cache}');
    while ($fnc = db_fetch_object($result)) {
      if (!isset($votingapi_cache_functions[$fnc->function])) {
        $votingapi_cache_functions[$fnc->function] = $fnc->function;
      }
    }
  }
  return $votingapi_cache_functions;
}

function votingapi_handler_cache_vote_tags() {
  static $votingapi_cache_tags;
  if (!is_array($votingapi_cache_tags)) {
    $votingapi_cache_tags = array();
    $votingapi_cache_tags = array(VOTINGAPI_VALUE_DEFAULT_TAG => '-- Default criteria --');
    $result = db_query('SELECT DISTINCT tag FROM {votingapi_cache}');
    while ($fnc = db_fetch_object($result)) {
      if (!isset($votingapi_cache_tags[$fnc->tag])) {
        $votingapi_cache_tags[$fnc->tag] = $fnc->tag;
      }
    }
  }
  return $votingapi_cache_tags;
}

function votingapi_handler_vote_tags() {
  static $votingapi_vote_tags;
  if (!is_array($votingapi_vote_tags)) {
    $votingapi_vote_tags = array(VOTINGAPI_VALUE_DEFAULT_TAG => '-- Default criteria --');
    $result = db_query('SELECT DISTINCT tag FROM {votingapi_vote}');
    while ($fnc = db_fetch_object($result)) {
      if (!isset($votingapi_cache_tags[$fnc->tag])) {
        $votingapi_vote_tags[$fnc->tag] = $fnc->tag;
      }
    }
  }
  return $votingapi_vote_tags;
}


function votingapi_views_handler_filter_uid_voted($op, $filter, $filterinfo, &$query) {
  $query->set_distinct(); // once you do this, there's just one node.
  $query->add_where("votingapi_vote.uid = '%s'", $filter['value']);
}