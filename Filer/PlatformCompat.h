#pragma once

/// <summary>
/// Platform�ˑ�����
/// </summary>
class PlatformCompat
{
public:
	static int ExecuteAssociatedApp(QString itemPath);
	static int MoveToTrash(QList<QString> itemPathList);
};

