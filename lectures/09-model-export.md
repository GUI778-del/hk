# 第9讲：模型导出与可重复部署（Exporting Models）

讲义目标
- 掌握模型保存与导出的常用方法（state_dict、TorchScript、ONNX）
- 了解跨平台部署中常见问题（输入格式、动态维度、依赖）
- 学会写简单的导出脚本并验证导出模型的推理一致性

核心要点
1. 保存与加载：torch.save(model.state_dict()) vs torch.save(model)
2. TorchScript：trace 与 script 的使用场景与限制
3. ONNX 导出：op 支持、动态轴、导出后验证（onnxruntime）
4. 导出对输入预处理/后处理的约束（确保在部署端一致）
5. 版本管理：模型版本、语义化标签、元数据保存（超参、训练环境、数据版本）

导出示例（TorchScript trace）
```python
model.eval()
example = torch.randn(1, 3, 224, 224)
traced = torch.jit.trace(model, example)
traced.save("model_traced.pt")
```

验证 ONNX（onnxruntime）
```python
import onnxruntime as ort
ort_sess = ort.InferenceSession("model.onnx")
outputs = ort_sess.run(None, {"input": input_np})
```

练习（notebooks/09-export.ipynb）
- 练习1：把微调后的模型导出为 TorchScript 与 ONNX，比较导出文件大小与在 CPU 上的推理速度。
- 练习2：编写导出时记录元数据（模型架构、训练集版本、评估指标）的脚本。

参考
- PyTorch JIT 和 ONNX 官方文档
