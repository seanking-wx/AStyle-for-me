#pragma once
#include "ASTranslation.h"

namespace AStyle
{

class ChineseSimplified : public ASTranslation  // 中文（简体）
{
	public:
		ChineseSimplified(void)
		{
			addPair("Formatted  %s\n", L"格式化  %s\n");     // should align with unchanged
			addPair("Unchanged  %s\n", L"未改变  %s\n");     // should align with formatted
			addPair("Directory  %s\n", L"目录  %s\n");
			addPair("Exclude Directory  %s\n", L"排除目录  %s\n");
			addPair("Exclude File  %s\n", L"排除文件  %s\n");
			addPair("Exclude (unmatched)  %s\n", L"排除（无匹配项） %s\n");
			addPair(" %s formatted   %s unchanged   ", L" %s 格式化   %s 未改变   ");
			addPair(" seconds   ", L" 秒   ");
			addPair("%d min %d sec   ", L"%d 分 %d 秒   ");
			addPair("%s lines\n", L"%s 行\n");
			addPair("Using default options file %s\n", L"使用默认配置文件 %s\n");
			addPair("Opening HTML documentation %s\n", L"打开HTML文档 %s\n");
			addPair("Invalid option file options:", L"无效的配置文件选项:");
			addPair("Invalid command line options:", L"无效的命令行选项:");
			addPair("For help on options type 'astyle -h'", L"输入 'astyle -h' 以获得有关命令行的帮助");
			addPair("Cannot open options file", L"无法打开配置文件");
			addPair("Cannot open directory", L"无法打开目录");
			addPair("Cannot open HTML file", L"无法打开HTML文件");
			addPair("Command execute failure", L"执行命令失败");
			addPair("Command is not installed", L"未安装命令");
			addPair("Missing filename in %s\n", L"在%s缺少文件名\n");
			addPair("Recursive option with no wildcard", L"递归选项没有通配符");
			addPair("Did you intend quote the filename", L"你打算引用文件名");
			addPair("No file to process %s\n", L"没有文件可处理 %s\n");
			addPair("Did you intend to use --recursive", L"你打算使用 --recursive");
			addPair("Cannot process UTF-32 encoding", L"不能处理UTF-32编码");
			addPair("\nArtistic Style has terminated", L"\nArtistic Style 已经终止运行");
		}
};

}   // end of astyle namespace
