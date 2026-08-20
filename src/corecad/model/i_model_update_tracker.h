#pragma once

namespace corecad::model
{

    template<typename TModel>
    class i_model_update_tracker
    {
    public:
        virtual void notify_updating(const TModel& model) = 0;
    };
}
