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

        // 搜索侦听器
        $('[type=text]').keydown(function (e) {
            let curKey = e.which;
            if (curKey === 13) {
                // 回车
                let input = $(this).val();
                $('#input').text(input);
                request.post("/api/hash/", JSON.stringify({
                    data: input
                }))
                    .then((response) => {
                        let data = response.data;
                        $('#hash').text(data.data.hash);
                    })
                    .catch((e) => {
                        console.error("计算哈希失败：", input, e);
                        functions.modal("错误", "无法计算哈希，请检查网络连接！");
                    });
            }
        });
    });