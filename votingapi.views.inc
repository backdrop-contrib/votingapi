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
      'handler' => 'views_handler_sort',
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
      'handler' => 'views_handler_sort',
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
  function option_definition() {
    $options = parent::option_definition();
    $options['appearance'] = '';
    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $appearances = array('' => t('Default appearance'));
    $appearances += module_invoke_all('votingapi_views_formatter', 'value');

    if (count($appearances) > 1) {
      $form['appearance'] = array(
        '#type' => 'select',
        '#title' => t('Appearance'),
        '#options' => $appearances,
        '#default_value' => $this->options['appearance'],
        '#weight' => -5,
      );
    }
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
 * @}
 */

function _votingapi_views_value_types($table = 'votingapi_vote') {
  static $types;
  if (!isset($types)) {
    $types = array(
      'percent' => t('Percent'),
      'points' => t('Points')
    );
    $results = db_query("SELECT DISTINCT value_type FROM {" . $table . "}");
    while ($tmp = db_fetch_object($results)) {
      $types[$tmp->value_type] = drupal_ucfirst($tmp->value_type);
    }
    $types[''] = t('No filtering');
  }
  return $types;
}

function _votingapi_views_tags($table = 'votingapi_vote') {
  static $tags;
  if (!isset($tags)) {
    $tags = array(
      'vote' => t('Default vote'),
    );
    $results = db_query("SELECT DISTINCT tag FROM {" . $table . "}");
    while ($tmp = db_fetch_object($results)) {
      $tags[$tmp->tag] = drupal_ucfirst($tmp->tag);
    }
    $tags[''] = t('No filtering');
  }
  return $tags;
}

function _votingapi_views_functions() {
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

function votingapi_views_data_alter(&$views_data) {
  // Add relationship handlers for both tables, for any base tables currently
  // available to Views.

  $default_relationships = module_invoke_all('votingapi_views_content_types');
  $default_relationships += array('node' => array('content_table' => 'node', 'content_id_column' => 'nid'));

  foreach($default_relationships as $content_type => $data) {
    $views_data[$data['content_table']]['votingapi_vote']['relationship'] = array(
      'title' => 'Individual votes',
      'help' => "Votes cast by users on $content_type content.",
      'base' => 'votingapi_vote',
      'field' => 'content_id',
      'relationship field' => $data['content_id_column'],
      'handler' => 'votingapi_views_handler_relationship',
      'extra' => array(
        array(
          'field' => 'content_type',
          'value' => $content_type,
          'numeric' => FALSE
        ),
      ),
    );

    $views_data[$data['content_table']]['votingapi_cache']['relationship'] = array(
      'title' => 'Voting results',
      'help' => "The aggregate results of votes cast on $content_type content.",
      'base' => 'votingapi_cache',
      'field' => 'content_id',
      'relationship field' => $data['content_id_column'],
      'handler' => 'votingapi_views_handler_relationship',
      'extra' => array(
        array(
          'field' => 'content_type',
          'value' => $content_type,
          'numeric' => FALSE
        ),
      ),
    );
  }
}

/**
 * A custom join handler that connects arbitrary base tables to VotingAPI's data.
 *
 * The base relationship handler can only handle a single join. Some relationships
 * are more complex and might require chains of joins; for those, you must
 * utilize a custom relationship handler.
 *
 * Definition items:
 * - base: The new base table this relationship will be adding. This does not
 *   have to be a declared base table, but if there are no tables that
 *   utilize this base table, it won't be very effective.
 * - relationship table: The actual table this relationship operates against.
 *   This is analogous to using a 'table' override.
 * - relationship field: The actual field this relationsihp operates against.
 *   This is analogous to using a 'real field' override.
 * - label: The default label to provide for this relationship, which is
 *   shown in parentheses next to any field/sort/filter/argument that uses
 *   the relationship.
 */
class votingapi_views_handler_relationship extends views_handler_relationship {
  function option_definition() {
    $options = parent::option_definition();

    $label = !empty($this->definition['label']) ? $this->definition['label'] : $this->definition['title'];
    $options['label'] = array('default' => $label, 'translatable' => TRUE);

    $options['votingapi']['value_type'] = array('default' => NULL);
    $options['votingapi']['tag'] = array('default' => NULL);
    if ($this->definition['base'] == 'votingapi_cache') {
      $options['votingapi']['function'] = array('default' => NULL);
    }

    return $options;
  }

  /**
   * Default options form that provides the label widget that all fields
   * should have.
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $form['votingapi'] = array(
      '#type' => 'fieldset',
      '#collapsible' => FALSE,
      '#title' => t('Data filters'),
      '#description' => t('For each piece of content, many pieces of voting data may be saved. Use these options to specify exactly which types should be available via this relationship. <strong>Warning!</strong> Leaving any of these filters empty may result in multiple copies of each piece of content being displayed in listings.'),
      '#tree' => TRUE,
    );
    $form['votingapi']['value_type'] = array(
      '#title' => t('Value type'),
      '#type' => 'select',
      '#options' => _votingapi_views_value_types($this->definition['base']),
      '#default_value' => $this->options['votingapi']['value_type'],
    );
    $form['votingapi']['tag'] = array(
      '#title' => t('Vote tag'),
      '#type' => 'select',
      '#options' => _votingapi_views_tags($this->definition['base']),
      '#default_value' => $this->options['votingapi']['tag'],
    );

    if ($this->definition['base'] == 'votingapi_cache') {
      $form['votingapi']['function'] = array(
        '#title' => t('Aggregation function'),
        '#type' => 'select',
        '#options' => _votingapi_views_functions(),
        '#default_value' => $this->options['votingapi']['function'],
      );
    }
    else {

    }
  }

  /**
   * Called to implement a relationship in a query.
   */
  function query() {
    // Figure out what base table this relationship brings to the party.
    $table_data = views_fetch_data($this->definition['base']);

    $def = $this->definition;
    $def['table'] = $this->definition['base'];
    $def['field'] = 'content_id';
    $def['left_table'] = $this->table;
    $def['left_field'] = $this->field;
    if (!empty($this->options['required'])) {
      $def['type'] = 'INNER';
    }

    if (!empty($def['join_handler']) && class_exists($def['join_handler'])) {
      $join = new $def['join_handler'];
    }
    else {
      $join = new views_join();
    }

    // use a short alias for this:
    $alias = $def['table'] . '_' . $def['left_table'];

    foreach ($this->options['votingapi'] as $field => $value) {
      $def['extra'][] = array(
        'field' => $field,
        'value' => $value,
        'numeric' => FALSE
      );
      $alias .= '_'. $value;
    }

    $join->definition = $def;
    $join->construct();

    $this->ensure_my_table();

    $this->alias = $this->query->add_relationship($alias, $join, $this->definition['base'], $this->relationship);
  }
}
