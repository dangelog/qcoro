// SPDX-FileCopyrightText: 2021 Daniel Vrátil <dvratil@kde.org>
//
// SPDX-License-Identifier: MIT

#include "testobject.h"
#include "qcoro/timer.h"

class QCoroTimerTest : public QCoro::TestObject<QCoroTimerTest>
{
    Q_OBJECT

private:
    QCoro::Task<> testTriggers_coro(QCoro::TestContext context) {
        QTimer timer;
        timer.setInterval(100ms);
        timer.start();

        co_await timer;
    }

    QCoro::Task<> testDoesntBlockEventLoop_coro(QCoro::TestContext context) {
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

private Q_SLOTS:
    addTest(Triggers)
    addTest(DoesntBlockEventLoop)
    addTest(DoesntCoAwaitInactiveTimer)
    addTest(DoesntCoAwaitNullTimer)
};

QTEST_GUILESS_MAIN(QCoroTimerTest)

#include "qtimer.moc"