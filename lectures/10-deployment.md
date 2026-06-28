# 第10讲：部署实战：FastAPI + Docker + 监控

讲义目标
- 学会把导出的模型包装为可扩展的推理服务（FastAPI 示例）
- 掌握使用 Docker 构建镜像与部署服务的基础流程
- 了解基本的线上监控与日志策略（延迟、错误率、样本分布漂移检测）

核心要点
1. 构建推理 API：输入校验、批量请求、异步与同步设计选择
2. Docker 化：最小镜像、层次化构建、模型文件管理
3. 可观测性：记录延迟、错误、输入分布（Prometheus + Grafana 简述）
4. 负载与扩展：水平扩展、容器编排（Kubernetes 简介）
5. 灰度发布与滚更：蓝绿/金丝雀/流量切分
6. 安全性：认证、速率限制、输入防注入

工程示例（deployment/app.py 已提供）
- 增强点：增加 /health、/metrics、批量推理接口、异步队列（Celery / RabbitMQ）连接示例

练习（notebooks/10-deployment.ipynb）
- 练习1：用 Docker 构建并运行部署镜像，使用 curl 测试并记录响应时间。
- 练习2：在本地使用 docker-compose 增加 Prometheus 并绘制简单监控面板。

参考
- FastAPI 文档、Docker 最佳实践
- Kubernetes 入门指南（官方文档）
