#ifndef QATTITUDEINDICATOR_H
#define QATTITUDEINDICATOR_H

#include <QWidget>


typedef enum _en_global_definitions_
{
    sizeMax = 600,
    sizeMin = 200,
    numbRollLine = 13,
    numbPitchLine =8
} EN_GLOABL_DEFINITIONS;

typedef enum _en_types_attitude_
{
    smallRollLine = 0,
    normalRollLine,
    smallPitchLine,
    normalPitchLine
} EN_TYPES_ATTITUDE;

class qAttitudeIndicator : public QWidget
{
    Q_OBJECT

public:
    qAttitudeIndicator(QWidget *parent = 0);
    ~qAttitudeIndicator();
    void setRoll(qreal val) {roll  = val;}
    void setPitch(qreal val){pitch = val;}
    qreal getRoll() {return roll;}
    qreal getPitch(){return pitch;}
protected:
    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *event);
    void keyPressEvent(QKeyEvent *event);

private:
    void getRollLine(EN_TYPES_ATTITUDE type,QPoint* pFrom,QPoint* pTo);
    void getPitchLine(EN_TYPES_ATTITUDE type,quint32 index,QPoint* pFrom,QPoint* pTo);
    void initTargetChar();
    void resizeTargetChar();
    void initRollChar();
    void resizeRollChar();

    int size;
    QPoint rollPoint[numbRollLine][2];
    qreal  rollRotate[numbRollLine];
    QPoint pitchPoint[numbPitchLine][2];
    qreal roll;
    qreal pitch;
    QVector<QLine> target;
    QVector<QLine> rollPointer;
};

#endif // QATTITUDEINDICATOR_H
