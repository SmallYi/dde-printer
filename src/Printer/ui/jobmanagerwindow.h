/*
 * Copyright (C) 2019 ~ 2019 Deepin Technology Co., Ltd.
 *
 * Author:     Wei xie <xiewei@deepin.com>
 *
 * Maintainer: Wei xie  <xiewei@deepin.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef JOBMANAGERWINDOW_H
#define JOBMANAGERWINDOW_H

#include <DMainWindow>
#include <DDialog>
DWIDGET_USE_NAMESPACE

#include <QAbstractTableModel>
#include <QItemDelegate>
#include <QTableView>

DWIDGET_BEGIN_NAMESPACE
class DIconButton;
class DButtonBox;
class DButtonBoxButton;
DWIDGET_END_NAMESPACE

QT_BEGIN_NAMESPACE
class QMenu;
class QAction;
class QPoint;
class QAbstractItemView;
class QTimer;
class QLabel;
QT_END_NAMESPACE

class JobManagerWindow;

class JobsDataModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit JobsDataModel(QObject* parent=nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    int columnCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;

protected:
    unsigned int getActionStatus(int iRow) const;
    int getJobId(int iRow);
    int getHighestPriority();
    void setHighestPriority(int id, int iPriority);

    void setWhichJob(int which);
    void updateJobState(int id, int state, const QString &message);

signals:
    void signalJobsCountChanged(int count);

private:
    void sortJobs();

protected slots:
    void slotReflushJobsList();
    void slotReflushJobItems();

private:
    QList<QMap<QString, QVariant>>   m_jobs;
    int     m_iHighestPriority;
    QTimer*             m_reflushTimer;
    int                 m_iWhichJob;

    friend class JobManagerWindow;
    friend class JobListView;
};

class JobItemDelegate : public QItemDelegate
{
    Q_OBJECT

public:
    explicit JobItemDelegate(QObject* parent=nullptr);

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const Q_DECL_OVERRIDE;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const Q_DECL_OVERRIDE;
};

class JobListView : public QTableView
{
    Q_OBJECT

public:
    explicit JobListView(QWidget* parent=nullptr);

protected:
    void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent *event)  Q_DECL_OVERRIDE;
    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;

    void doItemAction(int jobId, unsigned int iAction);
    int getJobId(int iRow);
    bool askDeleteJobs(unsigned int flag);

    void processAction(int index);

    QString getActionName(unsigned int iAction);

    void setLabelContentVisable(bool bShow);

    friend class JobManagerWindow;

protected slots:
    void slotShowContextMenu(const QPoint &pos);
    void slotMenuTriggered(QAction *action);
    void slotShowTips();

private:
    QMenu*              m_contextMenu;
    QList<QAction*>     m_atctions;
    JobItemDelegate*    m_itemDelegate;
    QTimer*             m_tipsTimer;
    QPoint              m_tipsPos;
    QStringList         m_actionNames;
    QLabel*             m_label;
};

class  JobManagerWindow : public DMainWindow
{
    Q_OBJECT

public:
    explicit JobManagerWindow(QWidget* parent=nullptr);

protected:
    void createUi();
    void initUi();
    void initConnect();

protected slots:
    void slotJobsCountChanged(int count);
    void slotWhichBoxClicked(QAbstractButton *whichbut);
    void slotJobStateChanged(int id, int state, const QString &message);

private:
    JobListView*        m_jobsView;
    JobsDataModel*      m_jobsModel;
    DIconButton*        m_reflushBut;
    DButtonBox*         m_whichButBox;
    QList<DButtonBoxButton*>    m_whichList;
    QLabel*             m_jobCountLabel;
};

#endif//JOBMANAGERWINDOW_H
