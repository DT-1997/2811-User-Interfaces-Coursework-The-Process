//
//
//

#include "button.h"


void TheButton::init(TheButtonInfo* i) {
    setIcon( *(i->icon) );
    info =  i;
}


void TheButton::clicked() {
    emit jumpTo(info);
}
