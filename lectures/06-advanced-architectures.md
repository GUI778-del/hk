# 第6讲：深度学习进阶 —— CNN / RNN / Transformer 概览

讲义目标
- 理解卷积神经网络（CNN）、循环神经网络（RNN）与 Transformer 的基本结构与适用场景
- 掌握在 PyTorch 中使用 torchvision / transformers 的基本方法
- 了解常用架构设计技巧（残差连接、批归一化、注意力机制）

核心要点
1. CNN：卷积、池化、残差网络（ResNet）、Inception、EfficientNet 思路
2. RNN：RNN/GRU/LSTM 的用途与长短期依赖问题
3. Transformer：自注意力机制、编码器-解码器架构、BERT/GPT 系列简介
4. 预训练模型的微调方法（feature extraction vs fine-tuning）
5. 模型容量与推理延迟/内存权衡
6. 常见模块：BatchNorm、LayerNorm、Dropout、Skip Connections

代码示例：用 torchvision 加载预训练 ResNet 微调
```python
import torchvision.models as models
model = models.resnet18(pretrained=True)
# 替换最后一层
model.fc = torch.nn.Linear(model.fc.in_features, num_classes)
```

练习（notebooks/06-architectures.py）
- 练习1：用 ResNet-18 在小数据集上微调，比较只训练分类头 vs 全部参数微调的结果与训练时间。
- 练习2：实现一个小型 Transformer 编码器用于文本分类（可使用 torch.nn.TransformerEncoder）。

参考
- Vaswani et al., “Attention is All You Need”
- torchvision、Hugging Face Transformers 文档
