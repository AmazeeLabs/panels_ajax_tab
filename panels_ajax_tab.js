(function ($) {
  /**
   * JS related to the tabs in the Panels tabs.
   */
  Drupal.behaviors.panels_ajax_tabs = {
    attach: function (context) {
      $(function() {
        $('.panels-ajax-tab-tab:not(.panels-ajax-tabs-processed)')
          .click(function(){
            var target_id      = $(this).data('target-id');
            var panel_name     = $(this).data('panel-name');
            var entity_context = $(this).data('entity-context');
            $.ajax({
              url: '/panels_ajax_tab/' + panel_name + '/' + entity_context,
              datatype:'html'
            }).done(function(data) {
              $('#panels-ajax-tab-container-' + target_id).html(data);
              drupal.attachBehaviors($('#panels-ajax-tab-container-' + target_id));
            });
            $(this).parent().siblings().removeClass('active');
            $(this).parent().addClass('active');
          })
          .css('cursor','pointer')
          .addClass('panels-ajax-tabs-processed');
        // Trigger a click event on the first tab to load it
        $('.panels-ajax-tab-tab:not(.panels-ajax-tabs-first-loaded)').first()
          .trigger('click')
          .addClass('panels-ajax-tabs-first-loaded');
      });
    }
  };
})(jQuery);
