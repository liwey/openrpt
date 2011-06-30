/*
 * Copyright (c) 2002-2006 by OpenMFG, LLC
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * If you do not wish to be bound by the terms of the GNU General Public
 * License, DO NOT USE THIS SOFTWARE.  Please contact OpenMFG, LLC at
 * info@openmfg.com for details on how to purchase a commercial license.
 */


void QueryList::editQuery(Q3ListBoxItem* lbitem)
{
    //qDebug("QueryList::editQuery(%s)", lbitem->text());
    if(lbitem) {
        // run the editor dialog
        QuerySource * qs = qsList->get(lbitem->text());

        if(qs == 0) {
            //qDebug("QueryList::editQuery(): Could not find '%s' in querylist\n",lbitem->text().latin1());
            return;
        }

        QueryEditor qe(this);
        qe.tbName->setText(qs->name());
        qe.tbQuery->setText(qs->query());
        if(qe.exec() == QDialog::Accepted) {
            QString nname = qe.tbName->text();
            QString nquery = qe.tbQuery->text();
            if(qs->name() != nname) {
                // we changed the name of the query.
                // lets check to make sure we didn't change it to
                // something that already exists
                if(qsList->get(nname) != 0) {
                    // display an error to the user.
                    // for now we'll just send out a debug message
                    //qDebug("Cannot change a QuerySource name to a name that already exists.");
                    return;
                }
                //lbitem->setText(nname);
                lbQuerys->changeItem(nname, lbQuerys->index(lbitem));
            }
            qs->setName(nname);
            qs->setQuery(nquery);
        }
    }
}

void QueryList::btnEdit_clicked()
{
    // get the selected item if any then call editQuery(QListBoxItem)
    int idx = lbQuerys->currentItem();
    if(idx != -1) {
        editQuery(lbQuerys->item(idx));
    }
}


void QueryList::btnDelete_clicked()
{
    //qDebug("QueryList::btnDelete_clicked()");
    // get the selected item in the listbox them remove it
    // from the listbox and from the QueryList
    int idx = lbQuerys->currentItem();
    //qDebug("    idx = %d", idx);
    if(idx != -1) {
        Q3ListBoxItem * item = lbQuerys->item(idx);
        //qDebug("    item = %s",item->text().latin1());
        QuerySource * qs = qsList->remove(item->text());
        if(qs != NULL) {
            //qDebug("    qs = %s", qs->_name.latin1());
            delete qs;
        } else {
            //qDebug("    qs = NULL");
        }
        lbQuerys->removeItem(idx);
    }
    //qDebug("    completed");
}

void QueryList::btnAdd_clicked()
{
    // add a new querySource item
    QueryEditor qe(this);
    if(qe.exec() == QDialog::Accepted) {
        QString nname = qe.tbName->text();
        QString nquery = qe.tbQuery->text();
        QuerySource * qs = new QuerySource(nname, nquery);
        if(qsList->add(qs) == TRUE) {
            lbQuerys->insertItem(nname);
        } else {
            // The item was not inserted for some reason
            qDebug("Failed to insert into into QuerySourceList");
            delete qs;
        }
    }
}


void QueryList::init( QuerySourceList * qsl )
{
    qsList = qsl;
    lbQuerys->clear();
    for(unsigned int i = 0; i < qsList->size(); i++) {
        lbQuerys->insertItem(QString(qsList->get(i)->name()));
    }
}