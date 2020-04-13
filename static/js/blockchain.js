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

    // 获取首页内容
    blockchain.getAllBlocks()
        .then((data) => {
            data = blockchain.processData(data);
            functions.renderHbs($list, TEMPLATE_LIST, {
                blocks: data
            });
        });
});