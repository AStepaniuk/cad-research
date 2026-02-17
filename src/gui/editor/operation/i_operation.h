#pragma once

namespace gui { namespace editor { namespace operation {
    enum class action_handle_status { operation_finished, operation_continues, unhandled };

    class i_operation
    {
    public:
        virtual void start() = 0;
        virtual void stop() = 0;
        virtual action_handle_status handle_rollback() = 0;

        virtual action_handle_status handle_mouse_move(float mx, float my) = 0;
        virtual action_handle_status handle_left_mouse_click(float mx, float my) = 0;
    };
}}}
