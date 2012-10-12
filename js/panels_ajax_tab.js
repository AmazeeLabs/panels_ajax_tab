(function($) {
  Drupal.behaviors.panels_ajax_tabs = {
    attach: function(context) {
      $(function() {
        $('.panels-ajax-tab-tab:not(.panels-ajax-tabs-processed)', context)
            .click(function(e) {
              e.preventDefault();
              $(this).panels_ajax_tabs_trigger();
            })
            .css('cursor', 'pointer')
            .addClass('panels-ajax-tabs-processed');

        // Trigger a click event on the first tab to load it
        $('.panels-ajax-tab-tab:not(.panels-ajax-tabs-first-loaded)', context).first().trigger('click');
        $('.panels-ajax-tab-tab').addClass('panels-ajax-tabs-first-loaded', context);
      });
    }
  };
})(jQuery);


/**
 * Panels-ajax-tabs-trigger is a jquery plugin that can be triggered. 
 * A callback, to be called after content has been loaded, can optionally be passed
 */
(function($){
  $.fn.extend({ 
    panels_ajax_tabs_trigger: function(callback) {
      return this.each(function() {
        var container = $(this).parents('.panels-ajax-tab:first');
        if ($(container).data('loading') === true)
          return true;
        $(container).data('loading', true);

        var target_id = $(this).data('target-id');
        var panel_name = $(this).data('panel-name');
        var entity_context = $(this).data('entity-context');
        $.ajax({
          url: Drupal.settings.basePath + 'panels_ajax_tab/' + panel_name + '/' + entity_context,
          datatype: 'html',
          cache: true,
          beforeSend: function(xhr) {
            $('#panels-ajax-tab-container-' + target_id).html('<img src="' + Drupal.settings.basePath + Drupal.settings.panel_ajax_tab.path + '/images/loading.gif"/>');
          },
          error: function(jqXHR, textStatus, errorThrown) {
            $('#panels-ajax-tab-container-' + target_id).html('Error: ' + errorThrown);
            $(container).data('loading', false);
          }
        }).done(function(data) {
          $('#panels-ajax-tab-container-' + target_id).html(data);
          Drupal.attachBehaviors($('#panels-ajax-tab-container-' + target_id));
          $(container).data('loading', false);
          
          // Trigger optional callback
          if (callback) {
            callback.call(this);
          }
        });
        $(this).parent().siblings().removeClass('active');
        $(this).parent().addClass('active');
      });
    }
  });
})(jQuery);