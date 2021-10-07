#include "policytimeedit.h"

PolicyTimeEdit::PolicyTimeEdit(QWidget *parent) : QTimeEdit(parent)
{
    setDisplayFormat("hh:mm");
    setWrapping(true);
}

void PolicyTimeEdit::stepBy(int steps)
{
    if(currentSection() == MinuteSection){
        setTime(time().addSecs(600));
        return;
    }
    QTimeEdit::stepBy(steps);
}
