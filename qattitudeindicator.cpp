#include <QtGui>
#include <QDebug>
#include "qattitudeindicator.h"

qreal defaultsRollRotate[numbRollLine]= {270.0,30.0,15.0,15.0,10.0,10.0,10.0,10.0,10.0,10.0,15.0,15.0,30.0};
EN_TYPES_ATTITUDE defaultsTypeRoll[numbRollLine] = { normalRollLine,normalRollLine ,smallRollLine ,normalRollLine,
                                                     smallRollLine ,smallRollLine  ,normalRollLine,smallRollLine,
                                                     smallRollLine ,normalRollLine ,smallRollLine ,normalRollLine,
                                                     normalRollLine };

EN_TYPES_ATTITUDE defaultsTypePitch[numbPitchLine] = { normalPitchLine,smallPitchLine ,normalPitchLine,smallPitchLine,
                                                       smallPitchLine ,normalPitchLine,smallPitchLine, normalPitchLine};


qAttitudeIndicator::qAttitudeIndicator(QWidget *parent)
    : QWidget(parent)
{
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(1000);
    size = sizeMin;
    setMinimumSize(sizeMin,sizeMin);
    setMaximumSize(sizeMax,sizeMax);
    resize(size, size);

    // from 0 degrees;
    for(int i=0;i < numbRollLine;i++)
    {
        rollRotate[i] = defaultsRollRotate[i];
        getRollLine(defaultsTypeRoll[i],&rollPoint[i][0],&rollPoint[i][1]);
    }
    for(int i=0;i< numbPitchLine;i++)
        getPitchLine(defaultsTypePitch[i],i,&pitchPoint[i][0],&pitchPoint[i][1]);

    initTargetChar();
    initRollChar();
    roll = 0.0;
    pitch = 0.0;

}

qAttitudeIndicator::~qAttitudeIndicator()
{

}

void qAttitudeIndicator::initTargetChar()
{
    QLine line;
    line.setLine(-size/4,0,-size/16,0);
    target.append(line);
    line.setLine(-size/16,0,0,-size/32);
    target.append(line);
    line.setLine(0,-size/32,size/16,0);
    target.append(line);
    line.setLine(size/16,0,size/4,0);
    target.append(line);
}

void qAttitudeIndicator::resizeTargetChar()
{
    target.clear();
    initTargetChar();
}

void qAttitudeIndicator::initRollChar()
{
    QLine line;
    line.setLine(-size/32,14*size/32,0,15*size/32);
    target.append(line);
    line.setLine(0,15*size/32,size/32,14*size/32);
    target.append(line);
    line.setLine(size/32,14*size/32,-size/32,14*size/32);
    target.append(line);
}

void qAttitudeIndicator::resizeRollChar()
{
    rollPointer.clear();
    initRollChar();
}

void qAttitudeIndicator::getRollLine(EN_TYPES_ATTITUDE type,QPoint* pFrom,QPoint* pTo)
{
    quint8 ofs = (type == smallRollLine)? (size/40) : (size/20) ;
    pFrom->setY(size/2-ofs);
    pFrom->setX(0);
    pTo->setY(size/2);
    pTo->setX(0);
}

void qAttitudeIndicator::getPitchLine(EN_TYPES_ATTITUDE type, quint32 index, QPoint* pFrom,QPoint* pTo)
{
    int x,y;
    if(index>=4) index++;
    y = size*(0.25-index*0.0625);
    x = (type==smallPitchLine)? (3*size/32):(5*size/32);
    pFrom->setX(-x);
    pFrom->setY(y);
    pTo->setX(x);
    pTo->setY(y);
}

void qAttitudeIndicator::resizeEvent(QResizeEvent *event)
{
    size = qMin(width(),height());
    for(int i=0;i < numbRollLine;i++)
        getRollLine(defaultsTypeRoll[i],&rollPoint[i][0],&rollPoint[i][1]);
    for(int i=0;i< numbPitchLine;i++)
        getPitchLine(defaultsTypePitch[i],i,&pitchPoint[i][0],&pitchPoint[i][1]);
    resizeTargetChar();
    resizeRollChar();
}

void qAttitudeIndicator::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QPoint center(0,0);
    QPen whitePen(Qt::white);
    QPen blackPen(Qt::black);
    QBrush bgSky(QColor(48,172,220));
    QBrush bgGround(QColor(247,168,21));
    whitePen.setWidth(2);
    blackPen.setWidth(1);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.translate(width() / 2, height() / 2);
    int side = qMin(width(), height());
    painter.scale(side / (qreal)(size), side / (qreal)(size));
    painter.setPen(blackPen);
    painter.rotate(roll);
    painter.setBrush(bgSky);

    int y = 0.25*size*pitch/20.;

    int x = sqrt(size*size/4 - y*y);
    qreal gr = atan((double)(y)/x);
    gr = gr * 180./3.1415926;
    painter.drawChord(-side/2,-side/2,side,side,gr*16,(180-2*gr)*16);
    painter.setBrush(bgGround);
    painter.drawChord(-side/2,-side/2,side,side,gr*16,-(180+2*gr)*16);
    painter.setPen(whitePen);

    painter.drawLine(-x,-y,x,-y);
    painter.setPen(blackPen);
    painter.rotate(-180.);
    for(int i=0;i<numbRollLine;i++)
    {
        painter.rotate(rollRotate[i]);
        painter.drawLine(rollPoint[i][0],rollPoint[i][1]);
    }
    whitePen.setWidth(1);
    painter.setPen(whitePen);
    painter.rotate(-90.);
    for(int i=0;i<numbPitchLine;i++)
    {
        painter.drawLine(pitchPoint[i][0],pitchPoint[i][1]);
    }
    painter.rotate(-roll);
    blackPen.setWidth(3);
    painter.setPen(blackPen);
    painter.drawLines(target);
    painter.drawLines(rollPointer);
}

void qAttitudeIndicator::keyPressEvent(QKeyEvent *event)
 {
     switch (event->key())
     {
     case Qt::Key_Left:
         if(roll>-90.)
         roll -= 1.0;
         break;
     case Qt::Key_Right:
         if(roll<90.)
         roll += 1.0;
         break;
     case Qt::Key_Down:
         if(pitch>-20.)
         pitch -=1.0;
         break;
     case Qt::Key_Up:
         if(pitch<20.)
         pitch +=1.0;
         break;
     default: break;
//         QFrame::keyPressEvent(event);
     }
     update();
 }
