#include "stdafx.h"
#include "folderviewstyleditemdelegate.h"

#include <QStyleOptionViewItem>
#include <QModelIndex>
#include <QDebug>
#include <QPainter>
#include <QApplication>
#include "TabContentView.h"
#include "settings.h"

FolderViewStyledItemDelegate::FolderViewStyledItemDelegate(QObject *parent/* = Q_NULLPTR*/)
	: QStyledItemDelegate(parent)
{
}

void FolderViewStyledItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	QStyleOptionViewItem opt = option;
	opt.state &= ~QStyle::State_Selected;       // FolderModel �� TextColorRole�EBackgroundRole �� Brush ���g�p���邽�߁A�����ł� Selected �𖳌��ɂ���
	opt.state &= ~QStyle::State_HasFocus;       // �J�[�\���ʒu�̘g��\�����Ȃ��悤�ɂ��邽�߁A�����ł� Has_Focus �𖳌��ɂ���(Windows�p)

	QStyledItemDelegate::paint(painter, opt, index);

	auto parent = qobject_cast<TabContentView*>(this->parent());
	if (parent != Q_NULLPTR)
	{
		if (parent->currentIndex().row() == index.row())
		{
			// �J�[�\���ʒu���A���_�[���C���ŕ\��
			painter->save();
			QColor color = (option.state & QStyle::State_Active) ? QColor("#0000ff") : QColor("#cccccc");
			int width = 1;
			QPen pen(color, static_cast<qreal>(width));
			painter->setPen(pen);
			painter->drawLine(option.rect.bottomLeft(), option.rect.bottomRight());
			painter->restore();
		}
	}
}
