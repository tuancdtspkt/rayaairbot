#include "qjoystick.h"

QJoystick::QJoystick(QObject *parent, int joystickEventTimeout, bool doAutoRepeat, int repeatDelay, int repeatTriggerDelay)
  : QObject(parent)
{

  if ( SDL_Init(SDL_INIT_JOYSTICK) == 0 ) {
    connect(&joystickTimer, SIGNAL(timeout()), this, SLOT(processEvents()));
  } else {
    qWarning("ERROR: couldn't initialize SDL joystick support");
  }

  joystick = NULL;
  numAxes = numButtons = numHats = numTrackballs = 0;
  autoRepeat = doAutoRepeat;
  autoRepeatDelay = repeatDelay;
  autoRepeatTriggerDelay = repeatTriggerDelay;
  eventTimeout = joystickEventTimeout;

  joyList();

  connect(this, SIGNAL(axisValueChanged_(int,int)), this, SLOT(axisValueChanged__(int,int)));
}

QJoystick::~QJoystick()
{
  if ( isOpen() )
    close();

  SDL_Quit();
}

QStringList QJoystick::joyList() {
  joystickNames.clear();
  for (int i = 0; i < SDL_NumJoysticks(); i++)
    joystickNames.append(SDL_JoystickName(i));

  if(joystickNames.isEmpty())
      joystickNames.append("None");

  return joystickNames;
}

bool QJoystick::open(QString s)
{
  return open( joystickNames.indexOf(s) );
}

bool QJoystick::open(int stick)
{
  if ( isOpen() )
    close();

  if(SDL_NumJoysticks() == 0)
      return false;

  joystick = SDL_JoystickOpen(stick);
  if ( joystick ) {
    joystickNameSelected = stick + 1;
    numAxes = SDL_JoystickNumAxes(joystick);
    numButtons = SDL_JoystickNumButtons(joystick);
    numHats = SDL_JoystickNumHats(joystick);
    numTrackballs = SDL_JoystickNumBalls(joystick);
    qWarning("DEBUG: SDL joystick %d opened; axes = %d, buttons = %d, hats = %d, trackballs = %d", stick, numAxes, numButtons, numHats, numTrackballs);

    axis_old_values.clear();
    for(int i=0; i < numAxes; i++)
        axis_old_values.append(0);

    emit initJoystick(numAxes, numButtons, numHats, joystickNames.at(stick));
    joystickTimer.start(eventTimeout);
    return true;
  } else {
    qWarning("ERROR: couldn't open SDL joystick %d", stick);
    return false;
  }
}

void QJoystick::close()
{
  joystickTimer.stop();
  if ( joystick )
    SDL_JoystickClose(joystick);
  joystick = NULL;
  numAxes = numButtons = numHats = numTrackballs = 0;
}

void QJoystick::axisValueChanged__(int axis, int value) {
    if(axis_old_values[axis]==value)
        return;
    axis_old_values[axis] = value;
    emit axisValueChanged(axis, value);
}

void QJoystick::processEvents()
{
  if ( !isOpen() )
    return;

  SDL_JoystickUpdate();

  int i;
  for (i = 0; i < numAxes; i++) {
    Sint16 moved = SDL_JoystickGetAxis(joystick, i);
    if ( abs(moved) >= deadzones[i] ) {
      if ( (moved != axes[i]) ) {
        int deltaMoved = abs(axes[i] - moved);
        if ( deltaMoved >= sensitivities[i] )
          emit axisValueChanged_(i, moved);
        axes[i] = moved;
        axisRepeatTimers[i].restart();
      } else if (autoRepeat && moved != 0) {
        if ( axisRepeatTimers[i].elapsed() >= autoRepeatDelay ) {
          emit axisValueChanged_(i, moved);
          axes[i] = moved;
        }
      } else
        axisRepeatTimers[i].restart();
    } else
      emit axisValueChanged_(i, 0);
//    emit axisValueChanged_(i, moved);
  }
  for (i = 0; i < numButtons; i++) {
    Uint8 changed = SDL_JoystickGetButton(joystick, i);
    if ( (changed != buttons[i]) ) {
      emit buttonValueChanged(i, (bool) changed);
      buttons[i] = changed;
      buttonRepeatTimers[i].restart();
    } else if (autoRepeat && changed != 0) {
      if ( buttonRepeatTimers[i].elapsed() >= autoRepeatDelay ) {
	if( buttonRepeatTriggerTimers[i].elapsed() >= autoRepeatTriggerDelay ) {
          buttonRepeatTriggerTimers[i].restart();
          emit buttonValueChanged(i, (bool) changed);
          buttons[i] = changed;
	}
      }
    } else
      buttonRepeatTimers[i].restart();
  }
  for (i = 0; i < numHats; i++) {
    Uint8 changed = SDL_JoystickGetHat(joystick, i);
    if ( (changed != hats[i]) ) {
      emit hatValueChanged(i, changed);
      hats[i] = changed;
      hatRepeatTimers[i].restart();
    } else if (autoRepeat && changed != 0) {
      if ( hatRepeatTimers[i].elapsed() >= autoRepeatDelay ) {
        emit hatValueChanged(i, changed);
        hats[i] = changed;
      }
    } else
      hatRepeatTimers[i].restart();
  }
  for (i = 0; i < numTrackballs; i++) {
    int dx, dy;
    SDL_JoystickGetBall(joystick, i, &dx, &dy);
    if ( dx != 0 || dy != 0 )
      emit trackballValueChanged(i, dx, dy);
  }
}

int QJoystick::getAxisValue(int axis)
{
  if ( isOpen() ) {
    SDL_JoystickUpdate();
    return SDL_JoystickGetAxis(joystick, axis);
  } else
    return 0;
}
