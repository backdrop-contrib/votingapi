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
        'handler' => 'votingapi_views_value_display_handler',
        'addlfields' => array('value_type', 'tag', 'function'),
      ),
      "tag" => array(
        'name' => t("VotingAPI: Vote criteria"),
        'sortable' => true,
        'handler' => 'votingapi_views_criteria_display_handler',
        'addlfields' => array('value_type', 'value', 'function'),
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
        'handler' => 'votingapi_views_nullable_field_handler',
        'help' => "Filter by the type of result being calculated (Percentage, points, etc)",
      ),
      "tag" => array(
        'name' => "VotingAPI: Result criteria",
        'operator' => array("=" => "Is", "!=" => "Is Not"),
        'list' => "votingapi_handler_cache_vote_tags",
        'list-type' => "select",
         'handler' => 'votingapi_views_nullable_field_handler',
       'help' => "Filter by the criteria being voted on",
      ),
      "function" => array(
        'name' => "VotingAPI: Result function",
        'operator' => array("=" => "Is", "!=" => "Is Not"),
        'list' => "votingapi_handler_cache_vote_functions",
        'list-type' => "select",
        'handler' => 'votingapi_views_nullable_field_handler',
        'help' => "Filter by the function used to aggregate results (total number of votes, average vote, etc)",
      ),
      "value" => array(
        'name' => t("VotingAPI: Has been voted on"),
        'list' => array('NULL' => 'Voted'),
        'list-type' => "select",
        'operator' => array('IS NOT' => "Yes", 'IS' => "No"),
        'handler' => 'votingapi_views_value_is_nullable_handler',
        'help' => t("This will filter out nodes that have or have not been voted on."),
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
      "value" => array(
        'name' => t("VotingAPI: Individual vote criteria"),
        'sortable' => true,
        'handler' => 'votingapi_views_value_display_handler',
        'addlfields' => array('value_type', 'value', 'function', 'uid'),
      ),
      "value" => array(
        'name' => t("VotingAPI: Current user's vote"),
        'sortable' => true,
        'handler' => 'votingapi_views_value_display_handler',
        'queryhandler' => 'votingapi_views_handler_vote_uid_is_currentuser',
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
        'handler' => 'votingapi_views_nullable_field_handler',
        'help' => "Filter by the type of result being calculated (Percentage, points, etc)",
      ),
      "tag" => array(
        'name' => "VotingAPI: Vote criteria",
        'operator' => array("=" => "Is", "!=" => "Is Not"),
        'list' => "votingapi_handler_vote_tags",
        'list-type' => "select",
        'handler' => 'votingapi_views_nullable_field_handler',
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
  $query->add_where("votingapi_vote.uid = '%s'", $filter['value']);
}


function votingapi_views_value_is_nullable_handler($op, $filter, $filterinfo, &$query) {
  $tn = $query->add_table($filterinfo['table']);
  $tname = $query->get_table_name($filterinfo['table'], $tn);
  $query->add_where("$tname.value $filter[operator] NULL");
}


function votingapi_views_nullable_field_handler($op, $filter, $filterinfo, &$query) {
  $tn = $query->add_table($filterinfo['table']);
  $tname = $filter['field'];
  $fvalue = $filter['value'];
  $fop = $filter['operator'];
  $query->add_where("$tname $fop '$fvalue' OR $tname IS NULL");
}

function votingapi_views_value_display_handler($op, $filter, $value, &$query) {
  if (!isset($value)) {
    return t('no votes');
  }
  
  $vobj->value = check_plain($value);
  $vobj->value_type = $fielddata['tablename'] . "_"  . $fieldinfo['value_type'];
  $vobj->content_type = $fielddata['tablename'] . "_"  . $fieldinfo['content_type'];
  $vobj->content_id = $fielddata['tablename'] . "_"  . $fieldinfo['content_id'];
  $vobj->tag = $fielddata['tablename'] . "_"  . $fieldinfo['tag'];
  if (isset($fieldinfo['function'])) {
    $vobj->function = $fielddata['tablename'] . "_"  . $fieldinfo['function'];
  }
  if (isset($fieldinfo['uid'])) {
    $vobj->uid = $fielddata['tablename'] . "_"  . $fieldinfo['uid'];
  }
  $result = votingapi_invoke_votingapi($vobj, 'format value');

  if (isset($result[0])) {
    return $result[0];
  }
  else {
    return $value;
  }
}

function votingapi_views_criteria_display_handler($op, $filter, $value, &$query) {
  if (!isset($value)) {
    return t('no votes');
  }
  
  $vobj->value = check_plain($value);
  $vobj->value_type = $fielddata['tablename'] . "_"  . $fieldinfo['value_type'];
  $vobj->content_type = $fielddata['tablename'] . "_"  . $fieldinfo['content_type'];
  $vobj->content_id = $fielddata['tablename'] . "_"  . $fieldinfo['content_id'];
  $vobj->tag = $fielddata['tablename'] . "_"  . $fieldinfo['tag'];
  if (isset($fieldinfo['function'])) {
    $vobj->function = $fielddata['tablename'] . "_"  . $fieldinfo['function'];
  }
  if (isset($fieldinfo['uid'])) {
    $vobj->uid = $fielddata['tablename'] . "_"  . $fieldinfo['uid'];
  }
  
  $result = votingapi_invoke_votingapi($vobj, 'format criteria');

  if (isset($result[0])) {
    return $result[0];
  }
  else {
    return $value;
  }
}


function votingapi_views_handler_vote_uid_is_currentuser($op, $filter, $filterinfo, &$query) {
  global $user;
  $query->add_where("votingapi_vote.uid = '%s'", $user->uid);
}