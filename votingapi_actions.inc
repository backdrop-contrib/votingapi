<?php
/* $Id */

/**
 * Functions that integrate VotingAPI with the Actions module.
 * Allows VotingAPI-based modules to insert nested sets of conditionals
 * and actions to be executed whenever a voting result is fired off.
 */

// A helper function that loads content from type/id pairs.
function _votingapi_load_content($content_id = 0, $content_id = 'node') {
  switch ($content_type) {
    case 'node':
      $content = node_load($content_id);
      break;
    case 'comment':
      $content = _comment_load($content_id);
      break;
    case 'aggregator-item':
      $result = db_query('SELECT * FROM {aggregator_item} WHERE iid = %d', $content_id);
      $content = db_fetch_object($result);
      break;
    case 'user':
      $content = user_load(array('uid' => $content_id));
      break;
  }
  return $content;
}


// Called by the Voting API whenever a result is calculated.
// Other helper functions build the actions cache from the database.

function _votingapi_process_actions($content_id, $content_type, $votes, $results) {
  $data = cache_get('votingapi_actions');
  $action_sets = unserialize($data->data);

  $content = _votingapi_load_content($content_type, $content_id);
  if ($content == NULL) {
    return;
  }
  
  foreach ($action_sets as $action_set) {
    if ($action_set->content_type == $content_type) {
      $actions = array();
      _votingapi_process_action_set($content, $votes, $results, $action_set, $actions);
      foreach ($actions as $action) {
        actions_do($action, $content);
      }
    }
  }
}


// An internal utility function that calls itself recursively to evaluate a
// tree of voting action sets. $actions is passed in by references, and accumulates
// actions-to-initiate. The calling function is responsible for firing them off.

function _votingapi_process_action_set($content = NULL, $votes = array(), $results = array(), $action_set = NULL, ?$actions = array()) {
  // Here, we iterate through every rule. The value starts as true,
  // and a single false will trip it to failure state.
  $set_result = FALSE;
  foreach($action_set->rules as $rule) {
    $function = $rule->handler;
    if (function_exist($function)) {
      // this calls a handler with several ops. 'process' and 'input' are the two i've thought of.
      $rules_result = $function('process', $content, $votes, $results, $rule);
    }
    else {
      $rules_result = FALSE;
    }
    
    if ($subset->mask == 'AND') {
      if ($rules_result === FALSE) {
        // bail out to avoid unecessary processing.
        return FALSE;
      }
      else {
        // AND the set result and rule result together.
        $set_result = $set_result && $rules_result;
      }
    }
    else if ($subset->mask == 'OR') {
      // OR the set result and rule result together.
      $set_result = $set_result || $rules_result;
    }
  }
 
  if ($set_result == TRUE) {
    // Now check sub-actions.

    foreach($action_set->subsets as $subset) {
      // check the required flag of the subset. if it is, evaluate it.
      if ($subset->required == TRUE) {
        $set_result = $set_result && _votingapi_process_action_set($content, $votes, $results, $subset, $actions);
        if ($set_result == FALSE) {
          return FALSE;
        }
      }
    }
    
    if ($set_result == TRUE) {
      // It's still true after executing required subsets. Add the actions, then process optional subsets.
      foreach ($action_set->actions as $action) {
        $actions[] = $action;
      }
      
      foreach($action_set->subsets as $subset) {
        // now handle the non-required subsets
        if ($subset->required == FALSE) {
          _votingapi_process_action_set($content, $votes, $results, $subset, $actions);
        }
      }
    }
  }
 
  return $set_result;
}


function votingapi_vote_result_handler($op, $content, $votes, $results, $rule) {
  if ($op == 'process') {
    //  for this handler, $rule->value is a serialized array in the following format:
    //
    //    $value->value_types = array('percent');   // an array of 1-n value types.
    //    $value->tags = array('vote');             // an array of 1-n tags
    //    $value->function = 'average';             // a single aggregate function
    //    $value->value = array(                    // an array of key/value pairs indicating
    //      '<' => '90',                            //   the operator and comparison value
    //      '>=' => '70',                           //   to match incoming results to.
    //    );
    //
    //  In the example above, any aggregate vote result in which a piece of content receives an 
    //  average percentage vote between 75% and 90% would match. Obviously, the specific values
    //  will change based on the specific action. If one of the above values is NOT specified
    //  it will be skipped. 
  
    $value = unserialize($rule->value);
    
    $passed = FALSE;
    
    // loop through all the result objects and see if there's one that satisfies all the conditions.
    foreach ($results as $result) {
      if (isset($value->value_types) {
        if (!in_array($result->value_type, $value->value_types)) {
          continue;
        }
      }
  
      if (isset($value->tags) {
        if (!in_array($result->tag, $value->tags)) {
          continue;
        }
      }
      
      if (isset($value->function) {
        if ($result->tag != $value->function) {
          continue;
        }
      }
    
      if (isset($value->value) {
        foreach($value->value as $c_op => $c_val) {
          
          // sadly, this probably doesn't work.
          // if (!($result->value $c_op $c_val) {
          //   continue 2;
          // }
          
          switch ($c_op) {
            case '<' :
              if (!($result->value < $c_val)) {
                continue 2;
              }
              break;

            case '<=' :
              if (!($result->value <= $c_val)) {
                continue 2;
              }
              break;

            case '==' :
              if (!($result->value == $c_val)) {
                continue 2;
              }
              break;

            case '!=' :
              if (!($result->value != $c_val)) {
                continue 2;
              }
              break;

            case '>=' :
              if (!($result->value >= $c_val)) {
                continue 2;
              }

              break;

            case '>' :
              if (!($result->value > $c_val)) {
                continue 2;
              }
              break;
          }
        }
      }
      
      // if we get this far, one of the result records has passed successfully.
      $passed = TRUE;
      break;
    }
    
    return $passed;
  }
}