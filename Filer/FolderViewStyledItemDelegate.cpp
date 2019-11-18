#include "stdafx.h"
#include "folderviewstyleditemdelegate.h"

#include <QStyleOptionViewItem>
#include <QModelIndex>
#include <QDebug>
#include <QPainter>
#include <QApplication>
#include <QAbstractItemView>
#include "settings.h"

FolderViewStyledItemDelegate::FolderViewStyledItemDelegate(QObject *parent/* = Q_NULLPTR*/)
	: QStyledItemDelegate(parent)
{
}

void FolderViewStyledItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	QStyleOptionViewItem opt = option;
	opt.state &= ~QStyle::State_Selected;       // FolderModel の TextColorRole・BackgroundRole の Brush を使用するため、ここでは Selected を無効にする
	opt.state &= ~QStyle::State_HasFocus;       // カーソル位置の枠を表示しないようにするため、ここでは Has_Focus を無効にする(Windows用)

	QStyledItemDelegate::paint(painter, opt, index);

	auto parent = qobject_cast<QAbstractItemView*>(this->parent());
	if (parent != Q_NULLPTR && parent->currentIndex().row() == index.row())
	{
		// カーソル位置をアンダーラインで表示
		painter->save();
		QColor color = (option.state & QStyle::State_Active) ? QColor("#0000ff") : QColor("#ccccff");
		int width = 3;
		QPen pen(color, static_cast<qreal>(width));
		painter->setPen(pen);

		const QRect rect(option.rect);
		painter->drawLine(rect.topLeft(), rect.topRight());
		painter->drawLine(rect.bottomLeft(), rect.bottomRight());

		if (index.column() == 0)
		{
			painter->drawRect(QRect(rect.topLeft(), rect.bottomLeft()));
		}
		else if (index.column() == (parent->model()->columnCount() - 1))
		{
			painter->drawRect(QRect(rect.topRight(), rect.bottomRight()));
		}
		painter->restore();
	}
}

