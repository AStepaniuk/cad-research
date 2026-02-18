#pragma once

#include "i_operation.h"

#include <memory>

#include "document.h"
#include "floor_view.h"
#include "calc_tools.h"

namespace gui { namespace editor { namespace operation {
    class operation_idle : public i_operation
    {
        doc::document& _document;
        floor_view& _view;
        calc_tools& _tools;

        std::unique_ptr<i_operation> sub_operation;

    public:
        operation_idle(doc::document& doc, floor_view& v, calc_tools& _t);

        void start() override;
        void stop() override;

        void cancel() override;
        action_handle_status rollback() override;

        action_handle_status mouse_move(float mx, float my) override;
        action_handle_status left_mouse_click(float mx, float my) override;
    };
}}}
