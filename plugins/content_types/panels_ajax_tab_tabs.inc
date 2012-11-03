<?php

$plugin = array(
  'title'            => t('AJAX Tab - Tabs'),
  'admin title'      => 'panels_ajax_tab_tabs_content_type_admin_title',
  'admin info'       => 'panels_ajax_tab_tabs_content_type_admin_info',
  'description'      => t('List of Tabs to control a container'),
  'single'           => TRUE,
  'category'         => t('AJAX Tabs'),
  'required context' => array(
    new ctools_context_optional(t('Entity Context'), 'entity'),
  ),
  'render callback'  => 'panels_ajax_tab_tabs_render',
  'edit form'        => 'panels_ajax_tab_tabs_edit_form'
);

function panels_ajax_tab_tabs_render($subtype, $conf, $panel_args, &$contexts) {
  static $default_tab_container_id = 0;
  ctools_include('context');
  ctools_include('plugins', 'panels');
  $block = new stdClass();
  $block->title = '';
  $block->content = '';

  // Add required JavaScript and CSS
  drupal_add_js(drupal_get_path('module', 'panels_ajax_tab') . '/js/panels_ajax_tab.js');
  drupal_add_css(drupal_get_path('module','panels_ajax_tab') . '/css/panels_ajax_tab.css');

  // Generate the context string
  if (!$contexts[0]->data) {
    $context_string = 'none';
  }
  else {
    $entity_type = $contexts[0]->type[2];
    $entity_ids = entity_extract_ids($entity_type, $contexts[0]->data);
    $entity_id = array_shift($entity_ids);
    $context_string = $entity_type . ':' . $entity_id;
  }

  // sort and load the mini-panels
  $minipanels = array();
  uasort($conf['mini_panels'], function($a,$b) { return $a['weight'] - $b['weight']; });
  foreach ($conf['mini_panels'] as $panel_name => $panel_conf) {
    if ($panel_conf['selected']) {
      $minipanels[] = panels_mini_load($panel_name);
    }
  }

  // Render the tabs
  $block->content .= theme('panels_ajax_tab_tabs', array('minipanels' => $minipanels, 'tab_container_id' => $conf['container_id'], 'context_string' => $context_string));

  return $block;
}

/**
 * Provide the administrative title of a Ajax tab.
 */
function panels_ajax_tab_tabs_content_type_admin_title($subtype, $conf) {
  
  return "Ajax Tab - " . $conf['container_id'];
}

/**
 * Display the administrative information for a Ajax tab .
 */
function panels_ajax_tab_tabs_content_type_admin_info($subtype, $conf) {
  $context =  $conf['context'];
  return panels_ajax_tab_tabs_render($subtype, $conf, array(), $context);
}

function panels_ajax_tab_tabs_edit_form($form, &$form_state) {
  $conf = $form_state['conf'];
  $panels = panels_mini_load_all();
  foreach($panels as $panel_name => $panel) {
    $panels[$panel_name] = (array) $panel;
    $panels[$panel_name] = empty($conf['mini_panels'][$panel_name]) ? $panels[$panel_name] : array_merge($panels[$panel_name],$conf['mini_panels'][$panel_name]);
  }
  if(!empty($conf['mini_panels'][$panel_name])) {
    uasort($panels, function($a,$b) { return $a['weight'] - $b['weight'];});
  }

  $form['mini_panels']['#tree'] = TRUE;
  foreach ($panels as $mini_panel) {
    $form['mini_panels'][$mini_panel['name']] = array(

      'selected' => array(
        '#type' => 'checkbox',
        '#default_value' => $mini_panel['selected'],
      ),

      'name' => array(
        '#markup' => $mini_panel['admin_title'],
      ),

      'weight' => array(
        '#type' => 'weight',
        '#title' => t('Weight'),
        '#default_value' => $mini_panel['weight'],
        '#delta' => 10,
        '#title-display' => 'invisible',
      ),
    );
  }

  $form['container_id'] = array(
    '#title' => t('Container ID'),
    '#type' => 'textfield',
    '#description' => 'If you are using tabs in seperate pane, you must supply a unique container id string.',
    '#default_value' => $conf['container_id'],
    '#required' => TRUE,
  );
  $form['#theme'] = array('panels_ajax_tab_tabs_edit_form');

  return $form;
}

function panels_ajax_tab_tabs_edit_form_submit(&$form, &$form_state) {
  $form_state['conf']['mini_panels'] = $form_state['values']['mini_panels'];
  $form_state['conf']['container_id'] = $form_state['values']['container_id'];
}
