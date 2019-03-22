#include "hugenholtz.h"

class GoldstoneDiagram : public Diagram
{
    GoldstoneDiagram(const HugenholtzDiagram& source);
    GoldstoneDiagram(const GoldstoneDiagram& rhs);
    GoldstoneDiagram& operator=(const GoldstoneDiagram& rhs);

    void indexLines();
}