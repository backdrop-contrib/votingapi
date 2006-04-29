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
        'query_handler' => 'votingapi_views_value_query_handler',
        'option' => array('#type' => 'vote_cache_properties'),
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
        'list' => "votingapi_cache_tags",
        'list-type' => "select",
        'handler' => 'votingapi_views_nullable_field_handler',
        'help' => "Filter by the tag being voted on",
      ),
      "function" => array(
        'name' => "VotingAPI: Result function",
        'operator' => array("=" => "Is", "!=" => "Is Not"),
        'list' => "votingapi_cache_functions",
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
        'query_handler' => 'votingapi_views_value_query_handler',
        'handler' => 'votingapi_views_value_display_handler',
        'option' => array('#type' => 'vote_properties'),
        'sortable' => true,
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
        'list' => "votingapi_vote_tags",
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

function votingapi_views_value_query_handler($field, $fieldinfo, &$query) {
  $table = $field['tablename'];
  
  if (is_string($field['options'])) {
    $options = unserialize($field['options']);
    if (is_array($options)) {
      foreach ($options as $column => $value) {
        $query->add_where("$table.$column = '$value'");
      }
    }
  }
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


/*
 *  Implements custom form widgets used by VotingAPI for its
 *  Views integration. It's rather wicked.
 */

function votingapi_elements() {
  $type['vote_properties'] = array(
    '#input' => TRUE,
    '#process' => array('expand_vote_properties' => array()),
    '#validate' => array('vote_properties_validate' => array()),
  );
  $type['vote_cache_properties'] = array(
    '#input' => TRUE,
    '#process' => array('expand_vote_cache_properties' => array()),
    '#validate' => array('vote_cache_properties_validate' => array()),
  );
  return $type;
}


function vote_properties_validate($form) {
  if (isset($form['value_type']['#value'])) {
    $val['value_type'] = trim($form['value_type']['#value']);
  }
  if (isset($form['tag']['#value'])) {
    $val['tag'] = trim($form['tag']['#value']);
  }

  form_set_value($form['tag'], NULL);
  form_set_value($form['value_type'], NULL);
  form_set_value($form, serialize($val));

  return $form;
}


function expand_vote_properties($element) { 
  $element['#tree'] = TRUE;
  $default_value = $element['#default_value'];
  if (isset($default_value) && !is_array($default_value)) {
    // we're in a funky situation where it's probably serialized.
    $default_value = unserialize($default_value);
  }

  $element['value_type'] = array(
    '#type' => 'select',
    '#options' => votingapi_cache_value_types(),
    '#default_value' => $default_value['value_type'],
    '#spawned' => TRUE,
  );

  $element['tag'] = array(
    '#type' => 'select',
    '#options' => votingapi_cache_tags(),
    '#default_value' => $default_value['tag'],
    '#spawned' => TRUE,
  );

  return $element;
}

function theme_vote_properties($element) {
  if ($element['#title'] || $element['#description']) {
    return theme('form_element', $element['#title'], '<div class="container-inline">' . $element['#children'] . '</div>', $element['#description'], $element['#id'], $element['#required'], form_get_error($element));
  }
  else {
    return '<div class="container-inline">' . $element['#children'] . '</div>';
  }
}

function vote_cache_properties_validate($form) {
  if (isset($form['value_type']['#value'])) {
    $val['value_type'] = trim($form['value_type']['#value']);
  }
  if (isset($form['tag']['#value'])) {
    $val['tag'] = trim($form['tag']['#value']);
  }
  if (isset($form['function']['#value'])) {
    $val['function'] = trim($form['function']['#value']);
  }

  form_set_value($form['tag'], NULL);
  form_set_value($form['value_type'], NULL);
  form_set_value($form['function'], NULL);
  form_set_value($form, serialize($val));

  return $form;
}


function expand_vote_cache_properties($element) { 
  $element['#tree'] = TRUE;
  $default_value = $element['#default_value'];
  if (isset($default_value) && !is_array($default_value)) {
    // we're in a funky situation where it's probably serialized.
    $default_value = unserialize($default_value);
  }

  $element['value_type'] = array(
    '#type' => 'select',
    '#options' => votingapi_cache_value_types(),
    '#default_value' => $default_value['value_type'],
    '#spawned' => TRUE,
  );

  $element['tag'] = array(
    '#type' => 'select',
    '#options' => votingapi_cache_tags(),
    '#default_value' => $default_value['tag'],
    '#spawned' => TRUE,
  );

  $element['function'] = array(
    '#type' => 'select',
    '#options' => votingapi_cache_functions(),
    '#default_value' => $default_value['function'],
    '#spawned' => TRUE,
  );
  return $element;
}

function theme_vote_cache_properties($element) {
  if ($element['#title'] || $element['#description']) {
    return theme('form_element', $element['#title'], '<div class="container-inline">' . $element['#children'] . '</div>', $element['#description'], $element['#id'], $element['#required'], form_get_error($element));
  }
  else {
    return '<div class="container-inline">' . $element['#children'] . '</div>';
  }
}