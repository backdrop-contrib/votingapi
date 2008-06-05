<?php

/* $Id$ */

/**
 * @file
 * Provide views data and handlers for votingapi.module
 */

/**
 * @defgroup views_votingapi_module votingapi.module handlers
 *
 * @{
 */

/**
 * Implementation of hook_views_data()
 */
function votingapi_views_data() {
  // Basic table information.

  // Define the base group of this table. Fields that don't
  // have a group defined will go into this field by default.
  $data['votingapi_vote']['table']['group']  = t('Voting API votes');
  $data['votingapi_cache']['table']['group']  = t('Voting API results');

  // For other base tables, explain how we join
  $data['votingapi_vote']['table']['join'] = array(
    'node' => array(
      'left_field' => 'nid',
      'field' => 'content_id',
      'extra' => array(
        array('field' => 'content_type', 'value' => 'node', 'numeric' => FALSE),
      ),
    ),
  );
  $data['votingapi_cache']['table']['join'] = array(
    'node' => array(
      'left_field' => 'nid',
      'field' => 'content_id',
      'extra' => array(
        array('field' => 'content_type', 'value' => 'node', 'numeric' => FALSE),
      ),
    ),
  );

  // ----------------------------------------------------------------
  // Fields

  // value
  $data['votingapi_vote']['value'] = array(
    'title' => t('Value'), // The item it appears as on the UI,
    'help' => t('The value of an individual cast vote.'), // The help that appears on the UI,
     // Information for displaying a title as a field
    'field' => array(
      'handler' => 'votingapi_views_handler_field_value',
      'click sortable' => TRUE,
     ),
     'filter' => array(
       'handler' => 'views_handler_filter_numeric',
       'label' => t('Value'),
       ),
    'sort' => array(
      'handler' => 'votingapi_views_handler_sort_value',
    ),
  );

  // value type
  $data['votingapi_vote']['value_type'] = array(
    'title' => t('Value type'), // The item it appears as on the UI,
    'help' => t('The nature of the vote being cast (points, percentage, etc).'), // The help that appears on the UI,
     // Information for displaying a title as a field
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
     ),
     'filter' => array(
       'handler' => 'views_handler_filter_string',
       'label' => t('Value type'),
       ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  // tag
  $data['votingapi_vote']['tag'] = array(
    'title' => t('Tag'), // The item it appears as on the UI,
    'help' => t('An optional tag to group multi-criteria votes.'), // The help that appears on the UI,
     // Information for displaying a title as a field
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
     ),
     'filter' => array(
       'handler' => 'views_handler_filter_string',
       'label' => t('Value type'),
       ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );


  // uid
  $data['votingapi_vote']['uid'] = array(
    'title' => t('User'), // The item it appears as on the UI,
    'help' => t('The user who cast the vote.'), // The help that appears on the UI,
     // Information for displaying a title as a field
    'field' => array(
      'handler' => 'views_handler_field_user',
      'click sortable' => TRUE,
     ),
     'filter' => array(
       'handler' => 'views_handler_filter_user_name',
       'label' => t('Value type'),
       ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );


  // vote_source
  $data['votingapi_vote']['vote_source'] = array(
    'title' => t('IP Address'), // The item it appears as on the UI,
    'help' => t('The IP address of the user who cast the vote.'), // The help that appears on the UI,
     // Information for displaying a title as a field
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
     ),
     'filter' => array(
       'handler' => 'views_handler_filter_string',
       'label' => t('Value type'),
       ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );


  // timestamp
  $data['votingapi_vote']['timestamp'] = array(
    'title' => t('Timestamp'), // The item it appears as on the UI,
    'help' => t('The time the vote was cast.'), // The help that appears on the UI,
     // Information for displaying a title as a field
    'field' => array(
      'handler' => 'views_handler_field_date',
      'click sortable' => TRUE,
     ),
     'filter' => array(
       'handler' => 'views_handler_filter_date',
       'label' => t('Value type'),
       ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );


  // Cache table fields

  // value
  $data['votingapi_cache']['value'] = array(
    'title' => t('Value'), // The item it appears as on the UI,
    'help' => t('The value of an individual cast vote.'), // The help that appears on the UI,
     // Information for displaying a title as a field
    'field' => array(
      'handler' => 'votingapi_views_handler_field_cache_value',
      'click sortable' => TRUE,
     ),
     'filter' => array(
       'handler' => 'views_handler_filter_numeric',
       'label' => t('Value'),
       ),
    'sort' => array(
      'handler' => 'votingapi_views_handler_sort_cache_value',
    ),
  );

  // value type
  $data['votingapi_cache']['value_type'] = array(
    'title' => t('Value type'), // The item it appears as on the UI,
    'help' => t('The nature of the results in question (points, percentage, etc).'), // The help that appears on the UI,
     // Information for displaying a title as a field
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
     ),
     'filter' => array(
       'handler' => 'views_handler_filter_string',
       'label' => t('Value type'),
       ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  // tag
  $data['votingapi_cache']['tag'] = array(
    'title' => t('Tag'), // The item it appears as on the UI,
    'help' => t('An optional tag to group multi-criteria results.'), // The help that appears on the UI,
     // Information for displaying a title as a field
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
     ),
     'filter' => array(
       'handler' => 'views_handler_filter_string',
       'label' => t('Value type'),
       ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );


  // function
  $data['votingapi_cache']['function'] = array(
    'title' => t('Function'), // The item it appears as on the UI,
    'help' => t('The aggregate function used to calculate the result.'), // The help that appears on the UI,
     // Information for displaying a title as a field
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
     ),
     'filter' => array(
       'handler' => 'views_handler_filter_string',
       'label' => t('Value type'),
       ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );


  // timestamp
  $data['votingapi_cache']['timestamp'] = array(
    'title' => t('Timestamp'), // The item it appears as on the UI,
    'help' => t('The time the results were calculated.'), // The help that appears on the UI,
     // Information for displaying a title as a field
    'field' => array(
      'handler' => 'views_handler_field_date',
      'click sortable' => TRUE,
     ),
     'filter' => array(
       'handler' => 'views_handler_filter_date',
       'label' => t('Value type'),
       ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );
  return $data;
}


class votingapi_views_handler_field_value extends views_handler_field_numeric {
  function settings_keys() {
    return array('value_type', 'tag');
  }

  /**
   * Ensure the main table for this handler is in the query. We do somewhat
   * debauched things here to allow separate copies of the table to be added
   * for each instance of this field.
   */
  function ensure_my_table() {
    if (!isset($this->table_alias)) {
      $join_extra[] = array('field' => 'content_type', 'value' => 'node', 'numeric' => FALSE);
      $alias = $this->table;

      foreach ($this->settings_keys() as $key) {
        if (!empty($this->options['vote'][$key])) {
          $join_extra[] = array('field' => $key, 'value' => $this->options['vote'][$key], 'numeric' => FALSE);
          $alias .= '_'. $this->options['vote'][$key];
        }
      }
      if (empty($alias)) {
        $alias = NULL;
      }

      $join = new views_join();
      $join->construct($this->table, 'node', 'nid', 'content_id', $join_extra);

      $this->table_alias = $this->query->add_table($this->table, $this->relationship, $join, $alias);
    }
    return $this->table_alias;
  }


  function option_definition() {
    $options = parent::option_definition();

    $options['appearance'] = '';

    $options['vote']['value_type'] = '';
    $options['vote']['tag'] = '';

    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $appearances = array('' => t('Default appearance'));
    $appearances += module_invoke_all('votingapi_views_value_formatter');

    if (count($appearances) > 1) {
      $form['appearance'] = array(
        '#type' => 'select',
        '#title' => t('Appearance'),
        '#options' => $appearances,
        '#default_value' => $this->options['appearance'],
        '#weight' => -5,
      );
    }

    $form['vote'] = array(
      '#type' => 'fieldset',
      '#collapsible' => FALSE,
      '#title' => t('Vote details'),
      '#description' => t('For each node, many vote values may be saved saved. Use these options to specify which one should appear in this field.'),
      '#weight' => -10,
      '#tree' => TRUE,
    );
    $form['vote']['value_type'] = array(
      '#title' => t('Result type'),
      '#type' => 'select',
      '#options' => _votingapi_views_vote_value_types(),
      '#default_value' => $this->options['vote']['value_type'],
    );
    $form['vote']['tag'] = array(
      '#title' => t('Result tag'),
      '#type' => 'select',
      '#options' => _votingapi_views_vote_tags(),
      '#default_value' => $this->options['vote']['tag'],
    );
  }

  function render($values) {
    $value = $values->{$this->field_alias};
    $function = $this->options['appearance'];
    if (!empty($function) && function_exists($function)) {
      return $function($value, $this, $values);
    }
    else {
      return parent::render($values);
    }
  }
}


/**
 * Field handler to add additional filters when a value is displayed.
 */
class votingapi_views_handler_field_cache_value extends votingapi_views_handler_field_value {
  function settings_keys() {
    return array('value_type', 'tag', 'function');
  }

  function option_definition() {
    $options = parent::option_definition();

    $options['vote']['function'] = '';

    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $form['vote']['function'] = array(
      '#title' => t('Result function'),
      '#type' => 'select',
      '#options' => _votingapi_views_cache_functions(),
      '#default_value' => $this->options['vote']['function'],
    );
  }
}


/**
 * @}
 */


function _votingapi_views_vote_value_types() {
  static $types;
  if (!isset($types)) {
    $types = array(
      'percent' => t('Percent'),
      'points' => t('Points')
    );
    $results = db_query("SELECT DISTINCT value_type FROM {votingapi_vote}");
    while ($tmp = db_fetch_object($results)) {
      $types[$tmp->value_type] = drupal_ucfirst($tmp->value_type);
    }
    $types[''] = t('No filtering');
  }
  return $types;
}

function _votingapi_views_vote_tags() {
  static $tags;
  if (!isset($tags)) {
    $tags = array(
      'vote' => t('Default vote'),
    );
    $results = db_query("SELECT DISTINCT tag FROM {votingapi_vote}");
    while ($tmp = db_fetch_object($results)) {
      $tags[$tmp->tag] = drupal_ucfirst($tmp->tag);
    }
    $tags[''] = t('No filtering');
  }
  return $tags;
}



function _votingapi_views_cache_value_types() {
  static $types;
  if (!isset($types)) {
    $types = array(
      'percent' => t('Percent'),
      'points' => t('Points')
    );
    $results = db_query("SELECT DISTINCT value_type FROM {votingapi_cache}");
    while ($tmp = db_fetch_object($results)) {
      $types[$tmp->value_type] = drupal_ucfirst($tmp->value_type);
    }
    $types[''] = t('No filtering');
  }
  return $types;
}

function _votingapi_views_cache_tags() {
  static $tags;
  if (!isset($tags)) {
    $tags = array(
      'vote' => t('Default vote'),
    );
    $results = db_query("SELECT DISTINCT tag FROM {votingapi_cache}");
    while ($tmp = db_fetch_object($results)) {
      $tags[$tmp->tag] = drupal_ucfirst($tmp->tag);
    }
    $tags[''] = t('No filtering');
  }
  return $tags;
}

function _votingapi_views_cache_functions() {
  static $functions;
  if (!isset($functions)) {
    $functions = array(
      'sum' => t('Sum'),
      'count' => t('Count'),
      'average' => t('Average'),
    );
    $results = db_query("SELECT DISTINCT function FROM {votingapi_cache}");
    while ($tmp = db_fetch_object($results)) {
      $functions[$tmp->function] = drupal_ucfirst($tmp->function);
    }
    $functions[''] = t('No filtering');
  }
  return $functions;
}



class votingapi_views_handler_sort_value extends views_handler_sort {
  function settings_keys() {
    return array('value_type', 'tag');
  }

  function option_definition() {
    $options = parent::option_definition();

    $options['vote']['value_type'] = '';
    $options['vote']['tag'] = '';

    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $form['vote'] = array(
      '#type' => 'fieldset',
      '#collapsible' => FALSE,
      '#title' => t('Vote result details'),
      '#description' => t('For each node, many vote result values are saved. Use these options to specify which one should be used to sort.'),
      '#weight' => -10,
      '#tree' => TRUE,
    );
    $form['vote']['value_type'] = array(
      '#title' => t('Result type'),
      '#type' => 'select',
      '#options' => _votingapi_views_cache_value_types(),
      '#default_value' => $this->options['vote']['value_type'],
    );
    $form['vote']['tag'] = array(
      '#title' => t('Result tag'),
      '#type' => 'select',
      '#options' => _votingapi_views_cache_tags(),
      '#default_value' => $this->options['vote']['tag'],
    );
  }

  /**
   * Ensure the main table for this handler is in the query. We do somewhat
   * debauched things here to allow separate copies of the table to be added
   * for each instance of this field.
   */
  function ensure_my_table() {
    if (!isset($this->table_alias)) {
      $join_extra[] = array('field' => 'content_type', 'value' => $view->base_table, 'numeric' => FALSE);
      $alias = $this->table;

      foreach ($this->settings_keys() as $key) {
        if (!empty($this->options['vote'][$key])) {
          $join_extra[] = array('field' => $key, 'value' => $this->options['vote'][$key], 'numeric' => FALSE);
          $alias .= '_'. $this->options['vote'][$key];
        }
      }
      if (empty($alias)) {
        $alias = NULL;
      }

      $join = new views_join();
      $join->construct($this->table, 'node', 'nid', 'content_id', $join_extra);

      $this->table_alias = $this->query->add_table($this->table, $this->relationship, $join, $alias);
    }
    return $this->table_alias;
  }

  function query() {
    $this->ensure_my_table();
    $this->query->add_orderby($this->table_alias, $this->real_field, $this->options['order']);
  }
}


class votingapi_views_handler_sort_cache_value extends votingapi_views_handler_sort_value {
  function settings_keys() {
    return array('value_type', 'tag', 'function');
  }

  function option_definition() {
    $options = parent::option_definition();

    $options['vote']['function'] = '';

    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $form['vote']['function'] = array(
      '#title' => t('Result function'),
      '#type' => 'select',
      '#options' => _votingapi_views_cache_functions(),
      '#default_value' => $this->options['vote']['function'],
    );
  }
}
