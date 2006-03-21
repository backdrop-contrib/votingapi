<?php
/* $Id */


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
        'addlfields' => array('value_type', 'tag', 'function', 'content_type', 'content_id'),
      ),
      "tag" => array(
        'name' => t("VotingAPI: Vote tag"),
        'sortable' => true,
        'handler' => 'votingapi_views_tag_display_handler',
        'addlfields' => array('value_type', 'value', 'function', 'content_type', 'content_id'),
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
          'percent' => 'Percentage',
          'points' => 'Points',
          'option' => 'Options',
        ),
        'list-type' => "select",
        'handler' => 'votingapi_views_nullable_field_handler',
        'help' => "Filter by the type of result being calculated (Percentage, points, etc)",
      ),
      "tag" => array(
        'name' => "VotingAPI: Result tag",
        'operator' => array("=" => "Is", "!=" => "Is Not"),
        'list' => "votingapi_handler_cache_vote_tags",
        'list-type' => "select",
        'handler' => 'votingapi_views_nullable_field_handler',
        'help' => "Filter by the tag being voted on",
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
        'name' => t("VotingAPI: Individual vote value"),
        'sortable' => true,
        'handler' => 'votingapi_views_value_display_handler',
        'sortable' => true,
        'addlfields' => array('value_type', 'tag', 'uid', 'content_type', 'content_id'),
      ),
      "tag" => array(
        'name' => t("VotingAPI: Individual vote tag"),
        'sortable' => true,
        'handler' => 'votingapi_views_tag_display_handler',
        'addlfields' => array('value_type', 'value', 'uid', 'content_type', 'content_id'),
      ),
      "timestamp" => array(
        'name' => t("VotingAPI: Individual vote timestamp"),
        'sortable' => true,
        'handler' => 
          array(
            "views_handler_field_date_small"=>"As Short Date", 
            "views_handler_field_date"=>"As Medium Date", 
            "views_handler_field_date_large"=>"As Long Date", 
            "views_handler_field_since" => "As Time Ago"
          ),
        'sortable' => true,
        'addlfields' => array('value_type', 'tag', 'uid', 'content_type', 'content_id'),
        'help' => "Display the time the vote was cast.",
      ),
      "uid" => array(
        'name' => t("VotingAPI: Individual vote user"),
        'handler' => 'votingapi_views_handler_field_username', 
        'sortable' => true, 
        'uid' => "uid", 
        'addlfields' => array('uid', 'content_type', 'content_id'),
      ),
    ),
    "sorts" => array(
      "value" => array('name' => "Vote value")
    ),
    "filters" => array(
      "value_type" => array(
        'name' => "VotingAPI: Individual vote type",
        'operator' => array("=" => "Is", "!=" => "Is Not"),
        'list' => array(
          'percent' => 'Percentage',
          'points' => 'Points',
          'option' => 'Custom',
        ),
        'list-type' => "select",
        'handler' => 'votingapi_views_nullable_field_handler',
        'help' => "Filter by the type of result being calculated (Percentage, points, etc)",
      ),
      "tag" => array(
        'name' => "VotingAPI: Individual vote tag",
        'operator' => array("=" => "Is", "!=" => "Is Not"),
        'list' => "votingapi_handler_vote_tags",
        'list-type' => "select",
        'handler' => 'votingapi_views_nullable_field_handler',
        'help' => "Filter by the tag being voted on",
      ),
      "uid" => array(
        'name' => "VotingAPI: Individual vote ",
        'operator' => array("=" => "was cast by"),
        'list' => array(NULL => "Current user"),
        'list-type' => "select",
        'handler' => 'votingapi_views_handler_filter_uid_voted',
      ),
      "currentuidtouched" => array(
        'field' => 'uid',
        'name' => "VotingAPI: Current user has voted",
        'operator' => array('=' => "voted on by"),
        'list' => "views_handler_filter_usercurrent",
        'list-type' => 'select',
        'handler' => "votingapi_views_handler_filter_uid_has_voted",
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
    $votingapi_cache_tags = array(VOTINGAPI_VALUE_DEFAULT_TAG => '-- Default tag --');
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
    $votingapi_vote_tags = array(VOTINGAPI_VALUE_DEFAULT_TAG => '-- Default tag --');
    $result = db_query('SELECT DISTINCT tag FROM {votingapi_vote}');
    while ($fnc = db_fetch_object($result)) {
      if (!isset($votingapi_cache_tags[$fnc->tag])) {
        $votingapi_vote_tags[$fnc->tag] = $fnc->tag;
      }
    }
  }
  return $votingapi_vote_tags;
}


function votingapi_views_handler_filter_uid_has_voted($op, $filter, $filterinfo, &$query) {
  $query->add_where("votingapi_vote.uid = '%s'", $filter['value']);
}

function votingapi_views_handler_filter_uid_voted($op, $filter, $filterinfo, &$query) {
  $query->add_where("votingapi_vote.uid $filter[operator] '%s' OR votingapi_vote.uid IS NULL", $filter['value']);
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
  $t = $filter['tablename'] . "_";
  $q = (array)$query;

  $vobj->value = check_plain($value);
  $vobj->value_type =  $q[$t . 'value_type'];
  $vobj->content_type = $q[$t . 'content_type'];
  $vobj->content_id = $q[$t . 'content_id'];
  $vobj->tag = $q[$t . 'tag'];
  if (isset($q[$t . 'function'])) {
    $vobj->function = $q[$t . 'function'];
  }
  if (isset($q[$t . 'uid'])) {
    $vobj->uid = $q[$t . 'uid'];
  }
  return votingapi_format_value($vobj);
}

function votingapi_views_tag_display_handler($op, $filter, $value, &$query) {
  if (!isset($value)) {
    return t('no votes');
  }
  $t = $filter['tablename'] . "_";
  $q = (array)$query;

  $vobj->value =  $q[$t . 'value'];
  $vobj->value_type =  $q[$t . 'value_type'];
  $vobj->content_type = $q[$t . 'content_type'];
  $vobj->content_id = $q[$t . 'content_id'];
  $vobj->tag = $q[$t . 'tag'];
  if (isset($q[$t . 'function'])) {
    $vobj->function = $q[$t . 'function'];
  }
  if (isset($q[$t . 'uid'])) {
    $vobj->uid = $q[$t . 'uid'];
  }
  return votingapi_format_tag($vobj);
}


function votingapi_views_handler_field_username($fieldinfo, $fielddata, $value, $data) {
  $uidfield = $fielddata['tablename'] . "_"  . $fieldinfo['uid'];
  $user = user_load(array('uid' => $data->$uidfield));
  return theme('username', $user);
}

function votingapi_views_default_views() {
  $view = new stdClass();
  $view->name = 'Node votes';
  $view->description = 'Lists all votes for a node';
  $view->access = array (
    0 => '2',
  );

  $view->page = TRUE;
  $view->disabled = TRUE;
  $view->page_title = '%1';
  $view->page_header = 'An overview of all votes cast for this piece of content.';
  $view->page_header_format = '1';
  $view->page_type = 'table';
  $view->url = 'node/$arg/votes';
  $view->use_pager = FALSE;
  $view->nodes_per_page = '10';
  $view->menu = TRUE;
  $view->menu_title = 'votes';
  $view->menu_tab = TRUE;
  $view->menu_tab_default = FALSE;
  $view->menu_weight = '';
  $view->sort = array (
  );
  $view->argument = array (
    array (
      'type' => 'nid',
      'argdefault' => '1',
      'title' => '',
      'options' => '',
    ),
  );
  $view->field = array (
    array (
      'tablename' => 'votingapi_vote',
      'field' => 'uid',
      'label' => 'user',
      'sortable' => '1',
    ),
    array (
      'tablename' => 'votingapi_vote',
      'field' => 'timestamp',
      'label' => 'time',
      'handler' => 'views_handler_field_since',
      'sortable' => '1',
      'defaultsort' => 'DESC',
    ),
    array (
      'tablename' => 'votingapi_vote',
      'field' => 'tag',
      'label' => 'criteria',
      'sortable' => '1',
    ),
    array (
      'tablename' => 'votingapi_vote',
      'field' => 'value',
      'label' => 'vote',
      'sortable' => '1',
    ),
  );
  $view->filter = array (
  );
  $view->requires = array(votingapi_vote);
  $views[$view->name] = $view;

  $view = new stdClass();
  $view->name = 'Moderation queue';
  $view->description = 'Nodes in the moderation queue';
  $view->access = array (
    0 => '2',
  );
  $view->page = TRUE;
  $view->disabled = TRUE;
  $view->page_title = 'moderation queue';
  $view->page_header = 'Content in the moderation queue can be voted on by the community. Depending on the results, content may be removed from the site or promoted to the front page.';
  $view->page_header_format = '1';
  $view->page_type = 'table';
  $view->url = 'queue';
  $view->use_pager = TRUE;
  $view->nodes_per_page = '40';
  $view->menu = TRUE;
  $view->menu_title = 'moderation queue';
  $view->menu_tab = FALSE;
  $view->menu_tab_default = FALSE;
  $view->menu_weight = '';
  $view->sort = array (
  );
  $view->argument = array (
  );
  $view->field = array (
    array (
      'tablename' => 'node',
      'field' => 'type',
      'label' => 'type',
      'sortable' => '1',
    ),
    array (
      'tablename' => 'node',
      'field' => 'title',
      'label' => 'title',
      'handler' => 'views_handler_field_nodelink_with_mark',
      'sortable' => '1',
    ),
    array (
      'tablename' => 'users',
      'field' => 'name',
      'label' => 'author',
      'sortable' => '1',
    ),
    array (
      'tablename' => 'node',
      'field' => 'created',
      'label' => 'created on',
      'handler' => 'views_handler_field_since',
      'sortable' => '1',
      'defaultsort' => 'DESC',
    ),
    array (
      'tablename' => 'votingapi_cache',
      'field' => 'value',
      'label' => 'current rating',
      'sortable' => '1',
    ),
  );
  $view->filter = array (
    array (
      'tablename' => 'node',
      'field' => 'distinct',
      'operator' => '=',
      'options' => '',
      'value' => '',
    ),
    array (
      'tablename' => 'node',
      'field' => 'moderate',
      'operator' => '=',
      'options' => '',
      'value' => '1',
    ),
    array (
      'tablename' => 'votingapi_cache',
      'field' => 'tag',
      'operator' => '=',
      'options' => '',
      'value' => 'vote',
    ),
    array (
      'tablename' => 'votingapi_cache',
      'field' => 'function',
      'operator' => '=',
      'options' => '',
      'value' => 'average',
    ),
  );
  $view->requires = array(node, users, votingapi_cache);
  $views[$view->name] = $view;

  return $views;
}