#include "operation_idle.h"

using namespace gui::editor::operation;

operation_idle::operation_idle(doc::document &doc, floor_view &v)
    : _document { doc }
    , _view { v }
{
}

void operation_idle::stop()
{
    // do nothing
}

action_handle_status operation_idle::handle_rollback()
{
    return action_handle_status::operation_finished;
}

action_handle_status operation_idle::handle_mouse_move(float mx, float my)
{
    _document.hovered_wall_id = _view.get_wall(mx, my);

    return action_handle_status::operation_continues;
}
