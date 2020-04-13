#include <blockchain/Blockchain.h>
#include "Controller.h"
#include <network/Endpoint.h>
#include <network/GlogLogger.h>
#include <network/Serializer.h>
#include <blockchain/Blockchain.h>
#include <hash/Hash.h>
#include <huffman/Huffman.h>
#include <search/Search.h>
#include <fstream>

using namespace std;
using namespace restbed;
using json = nlohmann::json;

void BlockController::publish(restbed::Service &service) {
    /*
     * 获得单个区块
     * GET /api/block/:id/
     */
    Endpoint::httpGet("/api/block/{id: [0-9]*}/", HANDLE_LOGIC(session, response) {
        string strId = session.request->get_path_parameter("id");
        UInt id = ::stoi(strId);
        if (id < 0 || id >= Blockchain::size()) {
            response.code = 404;
            response.message = "请求的区块不存在！";
            return;
        }
        response.data = Blockchain::get(id);
    }).publish(service);

    /*
     * 获得解压区块
     * GET /api/block/:id/decompress/
     */
    Endpoint::httpGet("/api/block/{id: [0-9]*}/decompress/", HANDLE_LOGIC(session, response) {
        string strId = session.request->get_path_parameter("id");
        UInt id = ::stoi(strId);
        if (id < 0 || id >= Blockchain::size()) {
            response.code = 404;
            response.message = "请求的区块不存在！";
            return;
        }
        Serializer::decompressedBlock(response.data, Blockchain::get(id));
    }).publish(service);

    /*
     * 获得区块二进制
     * GET /api/block/:id/binary/
     */
    Endpoint::httpGet("/api/block/{id: [0-9]*}/binary/", HANDLE_LOGIC(session, response) {
        string strId = session.request->get_path_parameter("id");
        UInt id = ::stoi(strId);
        if (id < 0 || id >= Blockchain::size()) {
            response.code = 404;
            response.message = "请求的区块不存在！";
            return;
        }
        Serializer::binaryBlock(response.data, Blockchain::get(id));
    }).publish(service);

    //
    auto apiBlock = Endpoint();
    apiBlock.setPath("/api/block/");

    /*
     * 请求所有区块
     * GET /api/block/
     */
    apiBlock.setGetLogic(HANDLE_LOGIC(session, response) {
        response.data = json::array();
        for (auto &block : Blockchain::getGlobalChain())
            response.data.push_back(block.get().getHeader());
    });

    /*
     * 增加区块
     * POST /api/block/
     */
    apiBlock.setPostLogic(HANDLE_LOGIC(session, response) {
        auto &body = session.body;
        if (!body.contains("data")) {
            response.code = 400;
            response.message = "缺少参数data";
            return;
        }
        auto data = body["data"].get<string>();
        auto newId = Blockchain::create(data);
        response.data["blockId"] = newId;
    });
    apiBlock.publish(service);

    /*
     * 从文件增加区块
     * POST /api/block/file/
     */
    Endpoint::httpPost("/api/block/file/", HANDLE_LOGIC(session, response) {
        auto &body = session.body;
        if (!body.contains("filepath")) {
            response.code = 400;
            response.message = "缺少参数filepath";
            return;
        }
        // 读入文件
        auto filepath = body["filepath"].get<string>();
        std::ifstream stream(filepath);
        if (stream.is_open()) {
            json jsonData;
            stream >> jsonData;
            vector<UInt> ids;
            for (auto &passage : jsonData) {
                auto data = passage.get<string>();
                auto newId = Blockchain::create(data);
                ids.push_back(newId);
            }
            response.data["blockIds"] = ids;
        } else {
            response.code = 400;
            response.message = "文件不存在！";
        }
    }).publish(service);
}

void HashController::publish(restbed::Service &service) {
    /*
     * 计算字符串哈希
     * POST /api/hash/
     */
    Endpoint::httpPost("/api/hash/", HANDLE_LOGIC(session, response) {
        auto &body = session.body;
        if (!body.contains("data")) {
            response.code = 400;
            response.message = "缺少参数data";
            return;
        }
        auto data = body["data"].get<string>();
        auto result = Hash::run(data);
        response.data["hash"] = Serializer::uInt32Hex(result);
    }).publish(service);
}

void HuffmanController::publish(restbed::Service &service) {
    /*
     * 压缩数据
     * POST /api/huffman/compress/
     */
    Endpoint::httpPost("/api/huffman/compress/", HANDLE_LOGIC(session, response) {
        auto &body = session.body;
        if (!body.contains("data")) {
            response.code = 400;
            response.message = "缺少参数data";
            return;
        }
        auto data = body["data"].get<string>();
        auto result = Huffman::compress({ByteBuffer::str(data)});
        response.data["dictionary"] = result.dictionary;
        response.data["data"] = result.data[0];
    }).publish(service);

    /*
     * 解压数据
     * POST /api/huffman/decompress/
     */
    Endpoint::httpPost("/api/huffman/decompress/", HANDLE_LOGIC(session, response) {
        auto &body = session.body;
        if (!body.contains("dictionary")) {
            response.code = 400;
            response.message = "缺少参数dictionary";
            return;
        }
        if (!body.contains("data")) {
            response.code = 400;
            response.message = "缺少参数data";
            return;
        }
        auto data = body["data"].get<ByteBuffer>();
        auto dictionary = body["dictionary"].get<ByteBuffer>();
        auto result = Huffman::decompress(dictionary, data);
        response.data["data"] = Serializer::bufferRawStr(result);
    }).publish(service);
}

void ValidateController::publish(restbed::Service &service) {
    /*
     * 校验区块链是否被篡改
     * GET /api/validate/
     */
    Endpoint::httpGet("/api/validate/", HANDLE_LOGIC(session, response) {
        auto result = Blockchain::check();
        response.data["result"] = result;
    }).publish(service);

    /*
     * 校验某篇新闻是否被篡改
     * POST /api/validate/decompress/
     */
    Endpoint::httpPost("/api/validate/block/{id: .*}/", HANDLE_LOGIC(session, response) {
        string strId = session.request->get_path_parameter("id");
        UInt id = ::stoi(strId);
        auto &body = session.body;
        if (!body.contains("data")) {
            response.code = 400;
            response.message = "缺少参数data";
            return;
        }
        // 解析&校验参数
        if (id < 0 || id >= Blockchain::size()) {
            response.code = 404;
            response.message = "请求的区块不存在！";
            return;
        }
        auto data = body["data"].get<string>();
        // 调用逻辑
        auto result = Blockchain::validateNews(data, id);
        response.data["wrong"] = result;
    }).publish(service);
}

void SearchController::publish(restbed::Service &service) {

    /*
     * 关键词搜索
     * GET /api/search/
     */
    Endpoint::httpGet("/api/search/", HANDLE_LOGIC(session, response) {
        auto keyword = session.request->get_query_parameter("keyword", "");
        if (keyword.empty()) {
            response.code = 400;
            response.message = "参数keyword不可为空";
            return;
        }
        auto keywords = StringUtil::split(keyword, ",");
        auto result = Search::search(keywords);
        // 返回
        response.data = json::array();
        for (auto &blockId : result)
            response.data.push_back(Blockchain::get(blockId).getHeader());
    }).publish(service);
}
