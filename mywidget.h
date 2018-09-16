#pragma once

#include <QWidget>
#include <QVector>

class QTimer;

class MyWidget : public QWidget
{
    Q_OBJECT

public:
    enum class Result
    {
        NotFinished,
        Solved,
        NotSolvable
    };

    explicit MyWidget(QWidget *parent = Q_NULLPTR);
    void reset(int columns, int rows);
    Result compute();

protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

private:
    struct Node {
        bool wall;
        int x;
        int y;
        QVector<const Node *> connections;

        //algorithm specific
        qreal f;
        qreal g;
        qreal h;
        const Node *previous;
    };

    QVector<QVector<Node> > m_nodes;

    QVector<const Node *> m_openSet;
    QVector<const Node *> m_closedSet;
    const Node *m_start;
    const Node *m_end;
    QVector<const Node *> m_result;
};
