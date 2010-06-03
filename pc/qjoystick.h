#ifndef _QJOYSTICK_H_
#define _QJOYSTICK_H_

#include <QObject>
#include <QMap>
#include <QTime>
#include <QTimer>
#include <QStringList>
#include <QList>
#include <SDL/SDL.h>

#define EVENT_TIMEOUT		  50
#define AUTOREPEAT_DELAY	 250
#define AUTOREPEAT_TRIGGER_DELAY 100

class QJoystick : public QObject
{
  Q_OBJECT

  public:
    QStringList joystickNames;
    int joystickNameSelected;
    SDL_Joystick *joystick;
    int numAxes;
    int numButtons;
    int numHats;
    int numTrackballs;
    int eventTimeout;
    int autoRepeatDelay;
    int autoRepeatTriggerDelay;
    bool autoRepeat;
    QTimer joystickTimer;
    QMap<int, int> deadzones;
    QMap<int, int> sensitivities;

    QJoystick(QObject *parent = 0,
             int joystickEventTimeout = EVENT_TIMEOUT,
             bool doAutoRepeat = true,
             int autoRepeatDelay = AUTOREPEAT_DELAY,
	     int autoRepeatTriggerDelay = AUTOREPEAT_TRIGGER_DELAY);
    ~QJoystick();
    bool open(int);
    bool open(QString);
    void close();
    bool isOpen() { return joystick != NULL; }
    int getAxisValue(int);
    QStringList joyList();

  private:
    QMap<int, Sint16> axes;
    QMap<int, Uint8> buttons;
    QMap<int, Uint8> hats;
    QMap<int, QTime> axisRepeatTimers;
    QMap<int, QTime> buttonRepeatTimers;
    QMap<int, QTime> buttonRepeatTriggerTimers;
    QMap<int, QTime> hatRepeatTimers; 

    QList<int> axis_old_values;

  signals:
    void initJoystick(int axis, int buttons, int hats, QString name);
    void axisValueChanged_(int axis, int value);
    void axisValueChanged(int axis, int value);
    void buttonValueChanged(int button, bool value);
    void hatValueChanged(int hat, int value);
    void trackballValueChanged(int trackball, int deltaX, int deltaY);

  public slots:
    void axisValueChanged__(int axis, int value);
    void processEvents();
};

#endif
