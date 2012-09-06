<?php

$plugin = array(
  'title'            => t('AJAX Tab - Tabs'),
  'description'      => t('List of Tabs to control a container'),
  'single'           => TRUE,
  'category'         => t('AJAX Tabs'),
  'required context' => array(
    new ctools_context_optional(t('Entity Context'), 'entity'),
  ),
  'render callback'  => 'panels_ajax_tab_tabs_render',
  'edit form'        => 'panels_ajax_tab_tabs_edit_form'
);

function panels_ajax_tab_tabs_render($subtype, $conf, $panel_args, &$contexts){
  static $default_tab_container_id = 0;
  ctools_include('context');
  ctools_include('plugins', 'panels');
  $block = new stdClass();
  $block->title ='';
  $block->content = '';

  // Add required JavaScript and CSS
  drupal_add_js(drupal_get_path('module','panels_ajax_tab') . '/panels_ajax_tab.js');

  // Generate the context string
  if (!$contexts[0]->data) {
    $context_string = 'none';
  }
  else {
    $entity_type = $contexts[0]->type[2];
    $entity_id = array_shift(entity_extract_ids($entity_type, $contexts[0]->data));
    $context_string = $entity_type . ':' . $entity_id;
  }

  // Load the mini-panels
  $minipanels = array();
  foreach ($conf['mini_panels'] as $panel_name) {
    $minipanels[] = panels_mini_load($panel_name);
  }

  // Render the tabs
  $block->content .= theme('panels_ajax_tab_tabs', array('minipanels' => $minipanels, 'tab_container_id' => $conf['container_id'], 'context_string' => $context_string));

  return $block;
}

function panels_ajax_tab_tabs_edit_form($form, &$form_state){
  $conf = $form_state['conf'];

  $panels = panels_mini_load_all();

  $options = array();
  foreach ($panels as $panel) {
    $options[$panel->name] = $panel->admin_title;
  }

  $form['mini_panels'] = array(
    '#title' => t('Mini Panels'),
    '#type' => 'checkboxes',
    '#options' => $options,
    '#description' => 'Select the Mini Panels to be used as AJAX tabs',
    '#default_value' => $conf['mini_panels'],
    '#required' => TRUE,
  );
  $form['container_id'] = array(
    '#title' => t('Container ID'),
    '#type' => 'textfield',
    '#description' => 'If you are using tabs in seperate pane, you must supply a unique container id string.',
    '#default_value' => $conf['container_id'],
    '#required' => TRUE,
  );

  return $form;
}

function panels_ajax_tab_tabs_edit_form_submit(&$form, &$form_state){
  $form_state['conf']['mini_panels'] = array_filter($form_state['values']['mini_panels']);
  $form_state['conf']['container_id'] = $form_state['values']['container_id'];
}
