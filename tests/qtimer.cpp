// SPDX-FileCopyrightText: 2021 Daniel Vrátil <dvratil@kde.org>
//
// SPDX-License-Identifier: MIT

#include "testobject.h"

#include "qcorotimer.h"

class QCoroTimerTest : public QCoro::TestObject<QCoroTimerTest> {
    Q_OBJECT

private:
    QCoro::Task<> testTriggers_coro(QCoro::TestContext) {
        QTimer timer;
        timer.setInterval(100ms);
        timer.start();

        co_await timer;
    }

    QCoro::Task<> testQCoroWrapperTriggers_coro(QCoro::TestContext) {
        QTimer timer;
        timer.setInterval(100ms);
        timer.start();

        co_await qCoro(timer).waitForTimeout();
    }

    QCoro::Task<> testDoesntBlockEventLoop_coro(QCoro::TestContext) {
        QCoro::EventLoopChecker eventLoopResponsive;

        QTimer timer;
        timer.setInterval(500ms);
        timer.start();

        co_await timer;

        QCORO_VERIFY(eventLoopResponsive);
    }

    QCoro::Task<> testDoesntCoAwaitInactiveTimer_coro(QCoro::TestContext ctx) {
        ctx.setShouldNotSuspend();

        QTimer timer;
        timer.setInterval(1s);
        // Don't start the timer!

        co_await timer;
    }

    QCoro::Task<> testDoesntCoAwaitNullTimer_coro(QCoro::TestContext ctx) {
        ctx.setShouldNotSuspend();

        QTimer *timer = nullptr;

        co_await timer;
    }

    void testThenTriggers_coro(TestLoop &el) {
        QTimer timer;
        bool triggered = false;
        timer.start(10ms);
        qCoro(timer).waitForTimeout().then([&el, &triggered]() {
            triggered = true;
            el.quit();
        });
        el.exec();
        QVERIFY(triggered);
    }

private Q_SLOTS:
    addTest(Triggers)
    addTest(QCoroWrapperTriggers)
    addTest(DoesntBlockEventLoop)
    addTest(DoesntCoAwaitInactiveTimer)
    addTest(DoesntCoAwaitNullTimer)

    addThenTest(Triggers)
};

QTEST_GUILESS_MAIN(QCoroTimerTest)

#include "qtimer.moc"
