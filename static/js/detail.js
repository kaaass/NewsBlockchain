/**
 * 区块详细信息
 */
require([
        'jquery',
        'axios',
        'module.functions',
        'module.constants',
        'module.blockchain',
        'bootstrap'],
    function ($, request, functions, constants, blockchain, _) {

        const TEMPLATE_DETAIL = "detail";

        let $detail = $('main');
        let curId;

        // 解析Url
        if (functions.requestParams.has(constants.PARAM_ID)) {
            curId = functions.requestParams.get(constants.PARAM_ID);
        } else {
            functions.modal("错误", "缺少区块id！");
            return;
        }

        // 渲染数据
        // 获取物品详细信息
        blockchain.getDetailById(curId)
            .then(data => {
                // 渲染
                console.log(data);
                functions.renderHbs($detail, TEMPLATE_DETAIL, data);
            });
    });