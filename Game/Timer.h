#include "../Animation/Animatable.h"
#include "../UI Components/Counter.h"

class Timer final : public Animatable {
public:
  Timer(const int x, const int y): _secondsCounter(x + 43, y, 0, 2), _minutesCounter(x, y, 0, 2) {}
  void pause() {
    _isPaused = true;
  }
  void resume() {
    _isPaused = false;
    resetAge();
  }

void update() override {
  if(!_isPaused) {
    _elapsedTime += getDeltaTime();
    _secondsCounter.setNumber(_elapsedTime / 1000 % 60);
    _minutesCounter.setNumber(_elapsedTime / (60* 1000));
  }
    Animatable::update();
  }

void draw(sf::RenderTarget& target, const sf::RenderStates states) const override {
    target.draw(_secondsCounter, states);
    target.draw(_minutesCounter, states);
  }

bool isAnimationOver() const override {
    return false;
  }

  int getElapsedTime() {
    return _elapsedTime;
  }

  private:
    bool _isPaused = true;
    int _elapsedTime = 0;

    Counter _secondsCounter;
    Counter _minutesCounter;
};