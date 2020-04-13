/**
 * 搜索
 */
require([
        'jquery',
        'axios',
        'module.functions',
        'module.constants',
        'module.blockchain',
        'bootstrap'],
    function ($, request, functions, constants, blockchain, _) {

        const TEMPLATE_LIST = "block_lists";

        let $list = $('#block-list');

        // 搜索侦听器
        $('[type=text]').keydown(function (e) {
            let curKey = e.which;
            if (curKey === 13) {
                // 回车
                let keyword = $(this).val();
                if (keyword.length <= 0) {
                    functions.modal("提示", "搜索内容不能为空！");
                    return false;
                }
                $('h1').text('搜索结果 - ' + keyword);
                request.get("/api/search/", {
                    params: {
                        keyword: keyword
                    }
                })
                    .then((response) => {
                        let data = response.data;
                        let blocks = blockchain.processData(data.data);
                        functions.renderHbs($list, TEMPLATE_LIST, {
                            blocks: blocks
                        });
                    })
                    .catch((e) => {
                        console.error("获取搜索数据失败：", keyword, e);
                        functions.modal("错误", "无法获取数据数据，请检查网络连接！");
                    });
            }
        });
    });