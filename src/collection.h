﻿/* -*- C++ -*-

   This file declares the Collection class.

   $ Author: Mirko Boehm $
   $ Copyright: (C) 2004-2013 Mirko Boehm $
   $ Contact: mirko@kde.org
         http://www.kde.org
         http://creative-destruction.me $

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#ifndef JOBCOLLECTION_H
#define JOBCOLLECTION_H

#include "job.h"
#include "jobpointer.h"
#include "collection_p.h"

namespace ThreadWeaver
{

class Thread;
class CollectionExecuteWrapper;

namespace Private { class Collection_Private; }

/** A Collection is a vector of Jobs that will be queued together.
 * In a Collection, the order of execution of the elements is not specified.
 *
 * It is intended that the collection is set up first and then
 * queued. After queuing, no further jobs should be added to the collection.
 */
class THREADWEAVER_EXPORT Collection : public Job
{
public:
    Collection();
    Collection(Private::Collection_Private * d);
    ~Collection();
    /** Append a job to the collection.
     *
     * To use Collection, create the Job objects first, add them to the collection, and then queue it. After
     * the collection has been queued, no further Jobs are supposed to be added.
     *
     * @note Once the job has been added, execute wrappers can no more be set on it */
    virtual void addJob(JobPointer);

    /** Stop processing, dequeue all remaining Jobs.
     * job is supposed to be an element of the collection.
     */
    //FIXME remove job argument?
    void stop(ThreadWeaver::JobPointer job);

    /** Return the number of elements in the collection. */
    int elementCount() const;

    //FIXME remove
    /** @deprecated Use elementCount(). */
    int jobListLength() const;

    /** @brief Add the job to this collection by pointer. */
    Collection &operator<<(ThreadWeaver::JobInterface *job);

    /** @brief Add the job to this collection. */
    Collection &operator<<(const ThreadWeaver::JobPointer &job);
    Collection &operator<<(JobInterface &job);

protected:
    /** Overload to queue the collection. */
    void aboutToBeQueued_locked(QueueAPI *api) Q_DECL_OVERRIDE;

    /** Overload to dequeue the collection. */
    void aboutToBeDequeued_locked(QueueAPI *api) Q_DECL_OVERRIDE;

    /** Return a ref-erence to the job in the job list at position i. */
    JobPointer jobAt(int i);

    //FIXME remove
    /** Return the number of jobs in the joblist.
     *  Assumes that the mutex is being held.
     */
    virtual int jobListLength_locked() const;

protected:
    /** Overload the execute method. */
    void execute(JobPointer job, Thread *) Q_DECL_OVERRIDE;

    /** Overload run().
     * We have to. */
    void run(JobPointer self, Thread *thread) Q_DECL_OVERRIDE;

protected:
    friend class CollectionExecuteWrapper; //needs to access d()
    friend class Collection_Private;
    Private::Collection_Private* d();
    const Private::Collection_Private* d() const;
};

}

#endif
