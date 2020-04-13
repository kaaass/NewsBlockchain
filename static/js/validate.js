/**
 * 篡改检测
 */
require([
    'jquery',
    'axios',
    'module.functions',
    'module.blockchain',
    'bootstrap'], function ($, request, functions, blockchain, _) {

    const TEMPLATE_LIST = "diff_list";

    let $list = $('#diff-list');

    // 事件绑定
    let $content = $('#content'),
        $blockId = $('#blockId');

    $('#news').click(() => {
        $('#addModal').modal('show');
    });

    $('#block-add').click(() => {
        let content = $content.val(),
            blockId = $blockId.val();
        if (content.length <= 0) {
            functions.modal("错误", "待校验内容不能为空！");
            return;
        }
        if (blockId.length <= 0) {
            functions.modal("错误", "区块 ID 不能为空！");
            return;
        }
        $content.val("");
        $blockId.val("");
        request.post(`/api/validate/block/${blockId}/`, JSON.stringify({
            data: content
        }))
            .then((response) => {
                let data = response.data;
                if (data.status !== 'OK') {
                    console.error("校验区块错误：", blockId, data);
                    functions.modal("错误", data.message);
                    return null;
                }
                functions.renderHbs($list, TEMPLATE_LIST, {
                    diffs: data.data
                });
            });
    });

    $('#blockchain').click(() => {
        request.get("/api/validate/")
            .then((response) => {
                let data = response.data;
                if (data.status !== 'OK') {
                    console.error("校验区块错误：", blockId, data);
                    functions.modal("错误", data.message);
                    return null;
                }
                if (data.data.result) {
                    functions.modal("信息", "当前区块信息正常！");
                } else {
                    functions.modal("错误", "当前区块遭到篡改！");
                }
            });
    });
});