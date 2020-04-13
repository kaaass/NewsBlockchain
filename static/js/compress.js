/**
 * 哈希函数
 */
require([
        'jquery',
        'axios',
        'module.functions',
        'module.constants',
        'bootstrap'],
    function ($, request, functions, constants, _) {

        let compressData = null;

        // 事件绑定
        let $content = $('#content');

        $('#compress').click(() => {
            $('#addModal').modal('show');
        });

        $('#block-add').click(() => {
            let content = $content.val();
            if (content.length <= 0) {
                functions.modal("错误", "压缩数据不能为空！");
                return;
            }
            $content.val("");
            $('#input').text(content);
            request.post("/api/huffman/compress/", JSON.stringify({
                data: content
            }))
                .then((response) => {
                    let data = response.data;
                    compressData = data.data;
                    $('#result').text(JSON.stringify(data.data));
                })
                .catch((e) => {
                    console.error("压缩失败：", input, e);
                    functions.modal("错误", "无法压缩，请检查网络连接！");
                });
        });

        $('#decompress').click(() => {
            if (compressData == null) {
                functions.modal("错误", "请先进行压缩！");
                return;
            }
            request.post("/api/huffman/decompress/", JSON.stringify(compressData))
                .then((response) => {
                    let data = response.data;
                    $('#result-decomp').text(data.data.data);
                })
                .catch((e) => {
                    console.error("解压失败：", input, e);
                    functions.modal("错误", "无法解压，请检查网络连接！");
                });
        });
    });