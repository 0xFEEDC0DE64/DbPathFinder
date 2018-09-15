#include "mywidget.h"

//#define ACUTE_ANGLE

#include <QTimer>
#include <QPainter>
#include <QDebug>
#include <QLine>
#include <QLineF>
#include <QMessageBox>

MyWidget::MyWidget(QWidget *parent) :
    QWidget(parent),
    m_start(Q_NULLPTR),
    m_end(Q_NULLPTR)
{
}

void MyWidget::reset(int columns, int rows)
{
    m_nodes.resize(rows);
    for(int y = 0; y < rows; y++)
        m_nodes[y].resize(columns);

    for(int y = 0; y < m_nodes.count(); y++)
        for(int x = 0; x < m_nodes.at(y).count(); x++)
        {
            Node &node = m_nodes[y][x];
            node.wall = !(x == 0 && y == 0) &&
                        !(x == m_nodes.at(y).count() -1 && y == m_nodes.count() - 1) &&
                        qrand() < RAND_MAX / 3;
            node.x = x;
            node.y = y;
            node.connections.clear();
            node.f = 0;
            node.g = 0;
            node.h = 0;
            node.previous = 0;
        }

    for(const auto &vector : m_nodes)
        for(const auto &node : vector)
        {
            if(node.wall)
                continue;

            auto x = node.x;
            auto y = node.y;

            if(x > 0 && !m_nodes.at(y).at(x - 1).wall)
                m_nodes[y][x].connections.append(&m_nodes[y][x - 1]);
            if(x < m_nodes.at(y).count() - 1 && !m_nodes.at(y).at(x + 1).wall)
                m_nodes[y][x].connections.append(&m_nodes[y][x + 1]);
            if(y > 0 && !m_nodes.at(y - 1).at(x).wall)
                m_nodes[y][x].connections.append(&m_nodes[y - 1][x]);
            if(y < m_nodes.count() - 1 && !m_nodes.at(y + 1).at(x).wall)
                m_nodes[y][x].connections.append(&m_nodes[y + 1][x]);

#ifdef ACUTE_ANGLE
            if(x > 0 && y > 0 && !m_nodes.at(y - 1).at(x - 1).wall && !m_nodes.at(y).at(x - 1).wall && !m_nodes.at(y - 1).at(x).wall)
                m_nodes[y][x].connections.append(&m_nodes[y - 1][x - 1]);
            if(x < m_nodes.at(y).count() - 1 && y > 0 && !m_nodes.at(y - 1).at(x + 1).wall && !m_nodes.at(y).at(x + 1).wall && !m_nodes.at(y - 1).at(x).wall)
                m_nodes[y][x].connections.append(&m_nodes[y - 1][x + 1]);
            if(x > 0 && y < m_nodes.count() - 1 && !m_nodes.at(y + 1).at(x - 1).wall && !m_nodes.at(y).at(x - 1).wall && !m_nodes.at(y + 1).at(x).wall)
                m_nodes[y][x].connections.append(&m_nodes[y + 1][x - 1]);
            if(x < m_nodes.at(y).count() - 1 && y < m_nodes.count() - 1 && !m_nodes.at(y + 1).at(x + 1).wall && !m_nodes.at(y).at(x + 1).wall && !m_nodes.at(y + 1).at(x).wall)
                m_nodes[y][x].connections.append(&m_nodes[y + 1][x + 1]);
#endif
        }

    m_start = &m_nodes.constFirst().constFirst();
    m_end = &m_nodes.constLast().constLast();

    m_openSet = { m_start };
    m_closedSet.clear();
    m_result.clear();
}

MyWidget::Result MyWidget::compute()
{
    if(m_openSet.count())
    {
        int lowestFIndex = 0;
        for(int i = 1; i < m_openSet.count(); i++)
            if(m_openSet.at(i)->f < m_openSet.at(lowestFIndex)->f)
                lowestFIndex = i;

        auto current = m_openSet.at(lowestFIndex);

        {
            auto test = current;
            m_result.clear();
            do
            {
                m_result.append(test);
                test = test->previous;
            }
            while(test);
        }

        if(current != m_end)
        {
            m_openSet.removeAll(current);
            m_closedSet.append(current);

            for(const auto &neighbour : current->connections)
            {
                if(!m_closedSet.contains(neighbour))
                {
                    auto increasedG = current->g +
#ifdef ACUTE_ANGLE
                            QLineF(current->x, current->y, neighbour->x, neighbour->y).length();
#else
                            qAbs(current->x - neighbour->x) + qAbs(current->y - neighbour->y);
#endif
                    bool newPath = false;

                    if(m_openSet.contains(neighbour))
                    {
                        if(increasedG < neighbour->g)
                            newPath = true;
                    }
                    else
                    {
                        newPath = true;
                        m_openSet.append(neighbour);
                    }

                    if(newPath)
                    {
                        m_nodes[neighbour->y][neighbour->x].g = increasedG;
                        m_nodes[neighbour->y][neighbour->x].h =
#ifdef ACUTE_ANGLE
                                QLineF(neighbour->x, neighbour->y, m_end->x, m_end->y).length();
#else
                                qAbs(neighbour->x - m_end->x) + qAbs(neighbour->y - m_end->y);
#endif
                        m_nodes[neighbour->y][neighbour->x].f = neighbour->g + neighbour->h;
                        m_nodes[neighbour->y][neighbour->x].previous = current;
                    }
                }
            }
        }
        else
            return Result::Solved;
    }
    else
        return Result::NotSolvable;

    return Result::NotFinished;
}

void MyWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)

    if(!m_nodes.count())
        return;

    QPainter painter(this);

    painter.setPen(Qt::NoPen);

    auto boxHeight = height() / m_nodes.count();

    for(const auto &vector : m_nodes)
    {
        auto boxWidth = width() / vector.count();

        for(const auto &node : vector)
        {
            if(m_result.contains(&node))
                painter.setBrush(Qt::darkGreen);
            else if(m_closedSet.contains(&node))
                painter.setBrush(Qt::red);
            else if(m_openSet.contains(&node))
                painter.setBrush(Qt::green);
            else if(node.wall)
                painter.setBrush(Qt::black);
            else
                continue;

            painter.drawRect(node.x * boxWidth, node.y * boxHeight, boxWidth, boxHeight);
        }
    }

    painter.setPen(Qt::blue);

    for(const auto &vector : m_nodes)
    {
        auto boxWidth = width() / vector.count();

        for(const auto &node : vector)
            for(const auto &otherNode : node.connections)
                painter.drawLine((node.x * boxWidth) + (boxWidth / 2), (node.y * boxHeight) + (boxHeight / 2), (otherNode->x * boxWidth) + (boxWidth / 2), (otherNode->y * boxHeight) + (boxHeight / 2));
    }
}
