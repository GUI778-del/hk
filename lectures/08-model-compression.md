# 第8讲：模型压缩与加速（量化、蒸馏、剪枝）

讲义目标
- 了解常用的模型压缩技术与适用场景
- 学会在 PyTorch 中使用量化与蒸馏的基本流程
- 认识推理性能优化的工程实践（ONNX、TorchScript、TensorRT）

核心要点
1. 模型压缩方法：剪枝（pruning）、权重共享、低秩分解
2. 量化：静态量化、动态量化、量化感知训练（QAT）
3. 知识蒸馏：teacher-student 框架与蒸馏损失
4. 模型导出与加速工具：TorchScript、ONNX、TensorRT、OpenVINO
5. 延迟/吞吐量/精度三者的工程权衡

代码示例：动态量化（PyTorch）
```python
import torch.quantization
quantized_model = torch.quantization.quantize_dynamic(
    model, {torch.nn.Linear}, dtype=torch.qint8
)
```

练习（notebooks/08-compression.py）
- 练习1：对训练好的 MNIST/CIFAR 模型做动态量化，比较模型大小与推理延迟变化。
- 练习2：使用小型教师网络蒸馏学生网络，比较学生网络在原始与蒸馏训练下的性能。

参考
- PyTorch Quantization 文档
- 背景论文：Han et al., “Deep Compression”; Hinton et al., “Distilling the Knowledge in a Neural Network”
