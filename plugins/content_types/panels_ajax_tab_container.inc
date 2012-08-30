<?php

$plugin = array(
  'title'            => t('AJAX Tab Container'),
  'description'      => t('The AJAX content will be loaded within this pane'),
  'single'           => TRUE,
  'category'         => t('AJAX Tabs'),
  'required context' => array(
    new ctools_context_optional(t('Entity Context'), 'entity'),
  ),
  'render callback'  => 'panels_ajax_tab_container_render',
  'edit form'        => 'panels_ajax_tab_container_edit_form'
);

function panels_ajax_tab_container_render($subtype, $conf, $panel_args, &$contexts){
  static $tab_container_id = 0;
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
    $context_string = $contexts[0]->type[2]. ':' . $contexts[0]->argument;
  }

  // Render the tabs
  $tabs = array();
  foreach ($conf['mini_panels'] as $panel_name) {
    $mini = panels_mini_load($panel_name);
    $tabs[] = '<span class="panels-ajax-tab-tab" data-panel-name="'.$mini->name.'" data-target-id="' . $tab_container_id . '" data-entity-context="' . $context_string . '">' . $mini->admin_title . '</span>';
  }
  $block->content .= theme('item_list', array('items' => $tabs, 'attributes' => array('class' => array('tabs', 'inline'))));

  // Render the container
  $block->content .= '<div id="panels-ajax-tab-container-' . $tab_container_id . '" class="panels-ajax-tab-container"></div>';

  $tab_container_id++;

  return $block;
}

function panels_ajax_tab_container_edit_form($form, &$form_state){
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

  return $form;
}

function panels_ajax_tab_container_edit_form_submit(&$form, &$form_state){
  $form_state['conf']['mini_panels'] = array_filter($form_state['values']['mini_panels']);
}
