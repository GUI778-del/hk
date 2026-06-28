# 第3讲：数据准备与数据增强（Data Preparation & Augmentation）

讲义目标
- 理解数据清洗、数据质量检查与标签校验的重要性
- 掌握常见的数据增强技巧与 PyTorch 中的实现方法
- 学会构建可复用的 Dataset 与 DataLoader

核心要点
1. 数据质量检查（缺失值、异常值、数据分布）
2. 标签一致性与样本不平衡处理（重采样、类别加权）
3. 特征工程基础（数值/类别特征处理、归一化/标准化）
4. 图像/文本常用数据增强技巧
5. PyTorch Dataset 与 DataLoader 自定义实践
6. 数据流水线的性能优化（多线程、预取、缓存）

代码示例：自定义 Dataset（图像目录 + CSV 标签）
```python
from torch.utils.data import Dataset
from PIL import Image
import pandas as pd

class ImageCsvDataset(Dataset):
    def __init__(self, csv_path, img_dir, transform=None):
        self.df = pd.read_csv(csv_path)
        self.img_dir = img_dir
        self.transform = transform

    def __len__(self):
        return len(self.df)

    def __getitem__(self, idx):
        row = self.df.iloc[idx]
        img = Image.open(f"{self.img_dir}/{row['filename']}").convert('RGB')
        label = int(row['label'])
        if self.transform:
            img = self.transform(img)
        return img, label
```

PyTorch 常见变换（transforms）示例
- RandomCrop, RandomHorizontalFlip, ColorJitter, Normalize, RandomErasing

练习（实验 notebook: notebooks/03-data-augmentation.py）
- 练习1：对 MNIST/CIFAR-10 实现并比较多种增强策略的训练曲线与验证性能。
- 练习2：构造不平衡数据（例如把部分类别下采样），实现重采样与类别加权，比较效果。
- 报告要点：实验设置、度量（准确率/混淆矩阵/类别 F1）、结论。

性能建议
- 使用 num_workers>0 提升 DataLoader 性能（在内存允许下）
- 对大型数据集使用缓存或 LMDB/TFRecord 等格式
- 在训练时避免在主线程做大量 I/O 或昂贵变换；用 transform pipeline 或预处理保存结果

参考
- PyTorch Data loading 与 transforms 文档
- “Practical Deep Learning for Coders” 相关章节（数据增强实践）
