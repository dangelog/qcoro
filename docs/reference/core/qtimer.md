<!--
SPDX-FileCopyrightText: 2022 Daniel Vrátil <dvratil@kde.org>

SPDX-License-Identifier: GFDL-1.3-or-later
-->

# QTimer

{{ doctable("Core", "QCoroTimer") }}

```cpp
QTimer timer;
timer.start(1s);
co_await timer;
```

The QCoro frameworks allows `co_await`ing on [QTimer][qdoc-qtimer] object. The
co-awaiting coroutine is suspended, until the timer finishes, that is until
[`QTimer::timeout()`][qdoc-qtimer-timeout] signal is emitted.

The timer must be active. If the timer is not active (not started yet or already
finished) the `co_await` expression will return immediately.

To make it work, include `QCoroTimer` in your implementation.

```cpp
#include <QCoroTimer>
#include <chrono>

using namespace std::chrono_literals;

QCoro::Task<> MyClass::pretendWork() {
    // Creates and starts a QTimer that will tick every second
    QTimer timer;
    timer.setInterval(1s);
    timer.start();

    for (int i = 1; i <= 100; ++i) {
        // Wait for the timer to tick
        co_await timer;
        // Update the progress bar value
        mProgressBar->setValue(i);
        // And repeat...
    }
    // ... until the for loop finishes.
}
```

[qdoc-qtimer]: https://doc.qt.io/qt-5/qtimer.html
[qdoc-qtimer-timeout]: https://doc.qt.io/qt-5/qtimer.html#timeout
