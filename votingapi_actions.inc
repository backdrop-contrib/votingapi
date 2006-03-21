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
