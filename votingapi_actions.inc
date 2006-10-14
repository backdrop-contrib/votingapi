<?php
// $Id$

/*********************************************
 *  VOTINGAPI ACTION CONDITION HANDLERS
 *  TO BE USED BY OTHER MODULES AND USERS
 *********************************************/

function votingapi_actions_votingapi_action_handlers() {
  $handlers = array(
    'votingapi_vote_result_handler' => array(
      'name' => 'Vote result properties'
    ),
    'votingapi_vote_handler' => array(
      'name' => 'Individual vote properties'
    ),
    'votingapi_node_properties_handler' => array(
      'name' => 'Node properties'
    ),
  );
  return $handlers;
}

function votingapi_vote_result_handler($op, $content, $votes, $results, $rule) {
  if ($op == 'process') {
    //  for this handler, $rule->data is an array in the following format:
    //
    //    $value = array(
    //      'value_type' => 'percent',   // an array of 1-n value types.
    //      'tag' => 'vote',             // an array of 1-n tags
    //      'comparison' = '<',                  // the comparison operator
    //      'value' => '90',                     // the value to be compared
    //      ),
    //    );
    //
    //  In the example above, any aggregate vote result in which a piece of content receives an 
    //  average percentage vote between 75% and 90% would match. Obviously, the specific values
    //  will change based on the specific action. If one of the above values is NOT specified
    //  it will be skipped. 
  
    $data = (object)$rule->data;
    $passed = FALSE;
    
    // loop through all the result objects and see if there's one that satisfies all the conditions.
    foreach ($results as $result) {
      if (isset($data->value_type)) {
        if ($result->value_type != $data->value_type) {
          continue;
        }
      }
  
      if (isset($data->tag)) {
        if ($result->tag != $data->tag) {
          continue;
        }
      }
      
      if (isset($data->function)) {
        if ($result->function != $data->function) {
          continue;
        }
      }

      switch ($data->comparison) {
        case '<' :
          if (!($result->value < $data->value)) {
            continue;
          }
          break;

        case '<=' :
          if (!($result->value <= $data->value)) {
            continue;
          }
          break;

        case '==' :
          if (!($result->value == $data->value)) {
            continue;
          }
          break;

        case '!=' :
          if (!($result->value != $data->value)) {
            continue;
          }
          break;

        case '>=' :
          if (!($result->value >= $data->value)) {
            continue;
          }

          break;

        case '>' :
          if (!($result->value > $data->value)) {
            continue;
          }
          break;
      }

      // if we get this far, one of the result records has passed successfully.
      $passed = TRUE;
      break;
    }

    return $passed;
  }
  else if ($op == 'form') {
    $form['value_type'] = array(
      '#type' => 'select',
      '#options' => votingapi_cache_value_types(),
    );
  
    $form['tag'] = array(
      '#type' => 'select',
      '#options' => votingapi_cache_tags(),
    );
  
    $form['function'] = array(
      '#type' => 'select',
      '#options' => votingapi_cache_functions(),
    );
    $form['comparison'] = array(
      '#type' => 'select',
      '#options' => array('==' => 'Is', '!=' => 'Is not', '<' => 'Is less than', '>' => 'Is greater than'),
    );
    $form['value'] = array(
      '#type' => 'textfield',
      '#maxlength' => 10,
    );
    
    return $form;
  }
  else if ($op == 'name') {
    return array('votingapi_vote_result_handler' => 'Vote result properties');
  }
}


function votingapi_vote_handler($op, $content, $votes, $results, $rule) {
  if ($op == 'process') {
    //  for this handler, $rule->data is an array in the following format:
    //
    //    $value = array(
    //      'value_type' => 'percent',   // an array of 1-n value types.
    //      'tag' => 'vote',             // an array of 1-n tags
    //      'function' => 'average',             // a single aggregate function
    //      'comparison' = '<',                  // the comparison operator
    //      'value' => '90',                     // the value to be compared
    //      ),
    //    );
    //
    //  In the example above, any aggregate vote result in which a piece of content receives an 
    //  average percentage vote between 75% and 90% would match. Obviously, the specific values
    //  will change based on the specific action. If one of the above values is NOT specified
    //  it will be skipped. 
  
    $data = (object)$rule->data;
    $passed = FALSE;
    
    // loop through all the result objects and see if there's one that satisfies all the conditions.
    foreach ($results as $result) {
      if (isset($data->value_type)) {
        if ($result->value_type != $data->value_type) {
          continue;
        }
      }
  
      if (isset($data->tag)) {
        if ($result->tag != $data->tag) {
          continue;
        }
      }

      switch ($data->comparison) {
        case '<' :
          if (!($result->value < $data->value)) {
            continue;
          }
          break;

        case '<=' :
          if (!($result->value <= $data->value)) {
            continue;
          }
          break;

        case '==' :
          if (!($result->value == $data->value)) {
            continue;
          }
          break;

        case '!=' :
          if (!($result->value != $data->value)) {
            continue;
          }
          break;

        case '>=' :
          if (!($result->value >= $data->value)) {
            continue;
          }

          break;

        case '>' :
          if (!($result->value > $data->value)) {
            continue;
          }
          break;
      }

      // if we get this far, one of the result records has passed successfully.
      $passed = TRUE;
      break;
    }
    
    return $passed;
  }
  else if ($op == 'form') {
    $form['value_type'] = array(
      '#type' => 'select',
      '#options' => votingapi_vote_value_types(),
    );
  
    $form['tag'] = array(
      '#type' => 'select',
      '#options' => votingapi_vote_tags(),
    );
  
    $form['comparison'] = array(
      '#type' => 'select',
      '#options' => array('==' => 'Is', '!=' => 'Is not', '<' => 'Is less than', '>' => 'Is greater than'),
    );
    $form['value'] = array(
      '#type' => 'textfield',
      '#maxlength' => 10,
    );
    
    return $form;
  }
  else if ($op == 'name') {
    return array('votingapi_vote_handler' => 'Individual vote properties');
  }
}

function votingapi_node_properties_handler($op, $content = NULL, $votes = NULL, $results = NULL, $rule = NULL) {
  if ($op == 'process') {
    //  for this handler, $rule->data is a keyed array of comaprisons by node property name:
    //
    //  $rule->data = array(
    //    'status' => array('==' => 1),   // must be published
    //    'uid' => array('!=' => 1),      // not authored by the admin account
    //  );
    //
    //  The keys in the sub-array are comparison operators, and the values are the value to
    //  compare to. This is mainly useful for ensuring that a node hasn't yet been promoted
    //  before promoting it, etc. At present only == and != are supported by this handler.
  
    $property = $rule['data']['property'];
    $comparison = $rule['data']['comparison'];
    $value = $rule['data']['value'];;

    switch ($comparison) {
      case '==' :
        if (!($content->$property == $value)) {
          return FALSE;
        }
        break;

      case '!=' :
        if (!($content->$property != $value)) {
          return FALSE;
        }
        break;
    }

    return TRUE;
  }
  else if ($op == 'form') {
    $form['property'] = array(
      '#type' => 'textfield',
      '#maxlength' => 10,
    );
    $form['comparison'] = array(
      '#type' => 'select',
      '#options' => array('==' => 'Is', '!=' => 'Is not'),
    );
    $form['value'] = array(
      '#type' => 'textfield',
      '#maxlength' => 10,
    );
    
    return $form;
  }
  else if ($op == 'name') {
    return array('votingapi_node_properties_handler' => 'Node properties');
  }
}

/*********************************************
 *  VOTINGAPI IMPLEMENTED ACTIONS. SHOULD
 *  PROBABLY BE ADDED TO ACTIONS.MODULE
 *********************************************/

/**
 * Touches the creation date of a node. Useful for moderated nodes that should appear
 * 'fresh' as soon as they're promoted.
 */
function action_node_touch_created($op, $edit = array(), &$node) {
  switch($op) {
    case 'do':
      $node->created = time();
      if (!$edit['defer']) {
        node_save($node);
      }
      watchdog('action', t('Touched creation date of node id %id', array('%id' => intval($node->nid))));
      break;

    case 'metadata':
      return array(
        'description' => t('Touch node creation date'),
        'type' => t('Node'),
        'batchable' => true,
        'configurable' => false,
      );

    // return an HTML config form for the action
    case 'form':
      return '';

    // validate the HTML form
    case 'validate':
      return TRUE;

    // process the HTML form to store configuration
    case 'submit':
      return '';
  }
}

/**
 * Touches the change date of a node. Useful for moderated nodes that should appear
 * 'fresh' as soon as they're promoted.
 */
function action_node_touch_changed($op, $edit = array(), &$node) {
  switch($op) {
    case 'do':
      $node->changed = time();
      if (!$edit['defer']) {
        node_save($node);
      }
      watchdog('action', t('Touched change date of node id %id', array('%id' => intval($node->nid))));
      break;

    case 'metadata':
      return array(
        'description' => t('Touch node change date'),
        'type' => t('Node'),
        'batchable' => true,
        'configurable' => false,
      );

    // return an HTML config form for the action
    case 'form':
      return '';

    // validate the HTML form
    case 'validate':
      return TRUE;

    // process the HTML form to store configuration
    case 'submit':
      return '';
  }
}

/**
 * Sets the status of a comment to PUBLISHED.
 */
function action_comment_publish($op, $edit = array(), &$comment) {
  switch($op) {
    case 'do':
      $comment->status = COMMENT_PUBLISHED;
      comment_save((array)$comment);
      watchdog('action', t('Set comment id %id to Published', array('%id' => intval($comment->cid))));
      break;

    case 'metadata':
      return array(
        'description' => t('Publish comment'),
        'type' => t('Comment'),
        'batchable' => true,
        'configurable' => false,
      );

    // return an HTML config form for the action
    case 'form':
      return '';

    // validate the HTML form
    case 'validate':
      return TRUE;

    // process the HTML form to store configuration
    case 'submit':
      return '';
  }
}

function action_comment_unpublish($op, $edit = array(), &$comment) {
  switch($op) {
    case 'do':
      $comment->status = COMMENT_NOT_PUBLISHED;
      comment_save((array)$comment);
      watchdog('action', t('Set comment id %id to Unpublished', array('%id' => intval($comment->cid))));
      break;

    case 'metadata':
      return array(
        'description' => t('Unpublish comment'),
        'type' => t('Comment'),
        'batchable' => true,
        'configurable' => false,
      );

    // return an HTML config form for the action
    case 'form':
      return '';

    // validate the HTML form
    case 'validate':
      return TRUE;

    // process the HTML form to store configuration
    case 'submit':
      return '';
  }
}
