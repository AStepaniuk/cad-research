#include "wall.h"

std::ostream& domain::plan::model::operator<<(std::ostream &os, const wall &w)
{
    return os << static_cast<const corecad::model::model_base<wall>&>(w)
        << " a:" << w.axis << " w:" << w.width
        << " l:" << w.left << " r:" << w.right
        << " ss:" << w.start_stub << " es:" << w.end_stub;
}
