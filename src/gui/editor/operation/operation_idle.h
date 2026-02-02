#pragma once

#include "i_operation.h"

#include "document.h"
#include "floor_view.h"

namespace gui { namespace editor { namespace operation {
    class operation_idle : public i_operation
    {
        doc::document& _document;

        floor_view& _view;

    public:
        operation_idle(doc::document& doc, floor_view& v);

        void stop() override;
        action_handle_status handle_rollback() override;

        action_handle_status handle_mouse_move(float mx, float my) override;
    };
}}}
