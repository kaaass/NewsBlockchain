/**
 * 区块链展示
 */
require([
    'jquery',
    'module.functions',
    'module.blockchain',
    'bootstrap'], function ($, functions, blockchain, _) {

    const TEMPLATE_LIST = "block_lists";

    let $list = $('#block-list');

    // 渲染全部区块内容
    let render = () => {
        blockchain.getAllBlocks()
            .then((data) => {
                data = blockchain.processData(data);
                functions.renderHbs($list, TEMPLATE_LIST, {
                    blocks: data
                });
            });
    };

    // 获取区块内容
    render();

    // 事件绑定
    let $content = $('#content'),
        $filePath = $('#filePath');

    $('#btn-block-add').click(() => {
        $('#addModal').modal('show');
    });

    $('#block-add').click(() => {
        let content = $content.val();
        if (content.length <= 0) {
            functions.modal("错误", "区块内容不能为空！");
            return;
        }
        $content.val("");
        blockchain.addBlock(content)
            .then((id) => {
                functions.modal("信息", "添加区块成功！新区块ID为：" + id);
                render();
            });
    });

    $('#btn-batch-add').click(() => {
        $('#batchModal').modal('show');
    });

    $('#batch-add').click(() => {
        let path = $filePath.val();
        if (path.length <= 0) {
            functions.modal("错误", "路径不能为空！");
            return;
        }
        $filePath.val("");
        blockchain.importBlock(path)
            .then((ids) => {
                functions.modal("信息", "导入区块成功！共导入 " + ids.length + " 篇新闻。");
                render();
            });
    });
});