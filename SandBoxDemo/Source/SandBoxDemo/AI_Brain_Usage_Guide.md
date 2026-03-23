# AI大模型智能大脑子系统使用指南

## 概述

`IntelligentBrainSubsystem` 是一个虚幻引擎的 GameInstance 子系统，用于集成多个 AI 大模型 API，为游戏提供智能对话和问答功能。

## 支持的AI提供商

- **Kimi** (Moonshot AI) - 月之暗面
- **Qwen** (通义千问) - 阿里巴巴
- **DeepSeek** - 深度求索
- **Doubao** (豆包) - 字节跳动

## 快速开始

### 1. 获取子系统

在蓝图中获取子系统：
```
Get Game Instance → Get Subsystem (IntelligentBrainSubsystem)
```

### 2. 设置API密钥

首先需要设置对应AI提供商的API密钥：
```
Set API Key
├── Provider: Kimi/Qwen/DeepSeek/Doubao
└── APIKey: "your-api-key-here"
```

### 3. 提问

向AI提问并获得回答：
```
Thinking
├── Question: "你的问题内容"
└── Provider: Kimi/Qwen/DeepSeek/Doubao
```

## 详细使用示例

### 示例1：基本使用
```
// 设置Kimi API密钥
Set API Key (Provider: Kimi, APIKey: "sk-your-kimi-api-key")

// 提问
FString Answer = Thinking("请介绍一下虚幻引擎", EAIProvider::Kimi)
```

### 示例2：使用豆包
```
// 设置豆包API密钥
Set API Key (Provider: Doubao, APIKey: "sk-your-doubao-api-key")

// 向豆包提问
FString Answer = Thinking("你好，请介绍一下你自己", EAIProvider::Doubao)
```

### 示例3：异步使用
```
// 异步提问
ThinkingAsync("你好，请介绍一下自己", EAIProvider::Qwen, 
    [](const FString& Response) {
        UE_LOG(LogTemp, Log, TEXT("AI回答: %s"), *Response);
    }
)
```

### 示例4：快速测试
```
// 快速测试功能
FString TestResult = QuickTest("sk-your-api-key", EAIProvider::Doubao)
```

## API密钥获取

### Kimi (Moonshot AI)
1. 访问：https://platform.moonshot.cn/
2. 注册账号并登录
3. 在控制台获取API密钥

### Qwen (通义千问)
1. 访问：https://dashscope.aliyun.com/
2. 使用阿里云账号登录
3. 开通服务并获取API密钥

### DeepSeek
1. 访问：https://platform.deepseek.com/
2. 注册账号并登录
3. 在API管理页面获取密钥

### Doubao (豆包)
1. 访问：https://www.doubao.com/
2. 注册字节跳动账号并登录
3. 在开发者中心获取API密钥

## 蓝图节点说明

### 主要节点

1. **Thinking** - 同步提问
   - Input: Question (字符串), Provider (枚举)
   - Output: AI回答 (字符串)

2. **ThinkingAsync** - 异步提问
   - Input: Question (字符串), Provider (枚举), OnComplete (委托)
   - Output: 无（通过委托返回结果）

3. **SetAPIKey** - 设置API密钥
   - Input: Provider (枚举), APIKey (字符串)

4. **SetBaseURL** - 设置自定义API地址（可选）
   - Input: Provider (枚举), BaseURL (字符串)

5. **QuickTest** - 快速测试功能
   - Input: APIKey (字符串), Provider (枚举)
   - Output: 测试结果 (字符串)

## 错误处理

子系统会自动处理以下错误情况：

- **API密钥未设置**：返回提示信息
- **网络连接失败**：返回网络错误信息
- **API响应解析失败**：返回解析错误信息
- **API服务错误**：返回具体的错误消息

## 性能建议

1. **使用异步调用**：对于耗时操作，建议使用 `ThinkingAsync` 避免阻塞游戏线程
2. **缓存API密钥**：在游戏初始化时设置API密钥，避免重复设置
3. **错误重试**：对于网络错误，可以实现重试机制
4. **限制请求频率**：避免过于频繁的API调用

## 各AI提供商特点

### Kimi
- 支持长文本处理（128K上下文）
- 擅长中文理解和生成
- 响应速度较快

### Qwen
- 阿里巴巴出品
- 中文优化良好
- 支持多种专业领域

### DeepSeek
- 免费额度较高
- 支持代码生成
- 性价比优秀

### Doubao
- 字节跳动出品
- 中文对话优化
- 支持多模态能力

## 扩展开发

### 添加新的AI提供商

1. 在 `EAIProvider` 枚举中添加新的提供商
2. 实现对应的 `CallXXXAPI` 方法
3. 在 `Thinking` 方法中添加新的case分支

### 自定义请求参数

可以通过修改 `CallXXXAPI` 方法中的请求体来调整：
- 模型选择
- 最大token数量
- 温度参数
- 其他高级参数

## 注意事项

1. **API调用成本**：注意API调用的费用，合理控制使用频率
2. **网络延迟**：AI API调用需要网络连接，考虑网络延迟的影响
3. **内容安全**：对用户输入和AI输出进行适当的内容过滤
4. **数据隐私**：避免传输敏感个人信息

## 技术支持

如果在使用过程中遇到问题，可以：

1. 查看引擎日志输出
2. 检查API密钥是否正确
3. 验证网络连接
4. 参考对应AI提供商的API文档

## 版本信息

- 子系统版本：1.1.0（新增豆包支持）
- 支持引擎版本：UE5.4+
- 最后更新：2026-03-23