// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2014 The Bitcoin developers
// Copyright (c) 2014-2015 The Dash developers
// Copyright (c) 2015-2017 The PIVX developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "libzerocoin/Params.h"
#include "chainparams.h"
#include "random.h"
#include "util.h"
#include "utilstrencodings.h"

#include <assert.h>

#include <boost/assign/list_of.hpp>

using namespace std;
using namespace boost::assign;

struct SeedSpec6 {
    uint8_t addr[16];
    uint16_t port;
};

#include "chainparamsseeds.h"

/**
 * Main network
 */

//! Convert the pnSeeds6 array into usable address objects.
static void convertSeed6(std::vector<CAddress>& vSeedsOut, const SeedSpec6* data, unsigned int count)
{
    // It'll only connect to one or two seed nodes because once it connects,
    // it'll get a pile of addresses with newer timestamps.
    // Seed nodes are given a random 'last seen time' of between one and two
    // weeks ago.
    const int64_t nOneWeek = 7 * 24 * 60 * 60;
    for (unsigned int i = 0; i < count; i++) {
        struct in6_addr ip;
        memcpy(&ip, data[i].addr, sizeof(ip));
        CAddress addr(CService(ip, data[i].port));
        addr.nTime = GetTime() - GetRand(nOneWeek) - nOneWeek;
        vSeedsOut.push_back(addr);
    }
}

//   What makes a good checkpoint block?
// + Is surrounded by blocks with reasonable timestamps
//   (no blocks before with a timestamp after, none after with
//    timestamp before)
// + Contains no strange transactions
static Checkpoints::MapCheckpoints mapCheckpoints =
    boost::assign::map_list_of
    (0, uint256("0x000008d83ed83d3c080def52a2d534af4c663522b138ade392572ef0e7feb18e"))
    (9376, uint256("0x0000000000f2e91e3b337fb015a1201c3e3e42786faa71b34b5f5a77c9cf47a1"))
    (9862, uint256("0x00000000000bc99877cfad18caa0ce2fe5f3b9f9c1b847cb7b19d8f7baa8f460"))
    (11654, uint256("0x0000000000ec15d42ee26263cbbe634b3d93625cd425a334adab2ed40e7eb9fc"))
    (14969, uint256("0x00000000000af9906303c44150f8ef65a695fd4afffa40cf9a577090f2f9ad9c"));
static const Checkpoints::CCheckpointData data = {
    &mapCheckpoints,
    1523456907, // * UNIX timestamp of last checkpoint block
    18362,    // * total number of transactions between genesis and last checkpoint
                //   (the tx=... number in the SetBestChain debug.log lines)
    2000        // * estimated number of transactions per day after checkpoint
};

static Checkpoints::MapCheckpoints mapCheckpointsTestnet =
    boost::assign::map_list_of
    (0, uint256("0x00000dd89ea8ecc3b47b1d60520120aa23835539b615f16ada62187fa763cb18"));
static const Checkpoints::CCheckpointData dataTestnet = {
    &mapCheckpointsTestnet,
    1522479068,
    0,
    250};

static Checkpoints::MapCheckpoints mapCheckpointsRegtest =
    boost::assign::map_list_of
    (0, uint256("0x56c672f4a3339263e6a5882fa2123f57edfe0b9975d93bc0437ff05a930ba20b"));
static const Checkpoints::CCheckpointData dataRegtest = {
    &mapCheckpointsRegtest,
    1522479068,
    0,
    100};

libzerocoin::ZerocoinParams* CChainParams::Zerocoin_Params() const
{
    assert(this);
    static CBigNum bnTrustedModulus(zerocoinModulus);
    static libzerocoin::ZerocoinParams ZCParams = libzerocoin::ZerocoinParams(bnTrustedModulus);

    return &ZCParams;
}

class CMainParams : public CChainParams
{
public:
    CMainParams()
    {
        networkID = CBaseChainParams::MAIN;
        strNetworkID = "main";
        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 4-byte int at any alignment.
         */

        pchMessageStart[0] = 0x33;
        pchMessageStart[1] = 0xd9;
        pchMessageStart[2] = 0xa1;
        pchMessageStart[3] = 0xc1;

        vAlertPubKey = ParseHex("04d079ca4f186f5f6e892be81638f2647b1f9eec5e753d7b488302484a412c37e4a07062c0b44887b16991e2878416c26bee4729874ed55f845a809aaed9fecb23");
        nDefaultPort = 61317;
        bnProofOfWorkLimit = ~uint256(0) >> 20; // ELLI starting difficulty is 1 / 2^12
        nSubsidyHalvingInterval = 210000;
        nMaxReorganizationDepth = 100;

        //nEnforceBlockUpgradeMajority = 750;
        //nRejectBlockOutdatedMajority = 950;
        //nToCheckBlockUpgradeMajority = 1000;

        nMinerThreads = 0;
        nTargetTimespan = 1 * 60; // ELLI: 1 day
        nTargetSpacing = 1 * 60;  // ELLI: 1 minute
        nMaturity = 100;
        nMasternodeCountDrift = 20;
        nMaxMoneyOut = 21000000 * COIN;

        /** Height or Time Based Activations **/
        nLastPOWBlock = 259200;
        nModifierUpdateBlock = 1;
        nZerocoinStartHeight = nLastPOWBlock + 100;
        nZerocoinStartTime = 1522479067 + nZerocoinStartHeight*nTargetSpacing; 
        nBlockEnforceSerialRange = 1; //Enforce serial range starting this block

        nBlockRecalculateAccumulators = nZerocoinStartHeight*3; //Trigger a recalculation of accumulators
        nBlockFirstFraudulent = nZerocoinStartHeight*3; //First block that bad serials emerged
        nBlockLastGoodCheckpoint = nZerocoinStartHeight*3; //Last valid accumulator checkpoint
        nBlockEnforceInvalidUTXO = nZerocoinStartHeight*3; //Start enforcing the invalid UTXO's
        nBlockEnforceMasternodePay = 15000; //Start enforcing mn pay

        const char* pszTimestamp = "elliotproject 31-03-2018 v2";
        CMutableTransaction txNew;
        txNew.vin.resize(1);
        txNew.vout.resize(1);
        txNew.vin[0].scriptSig = CScript() << 486604799 << CScriptNum(4) << vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
        txNew.vout[0].nValue = 250 * COIN;
        txNew.vout[0].scriptPubKey = CScript() << ParseHex("04b3e4906c8b37b9975dfcdba0543999991fff091a66a7bd5bb885ee3e64686dd9b36c92ed45cc1f5b52d374264a83c36556f4f5ed5d4950bd99e1643b67b38639") << OP_CHECKSIG;
        genesis.vtx.push_back(txNew);
        genesis.hashPrevBlock = 0;
        genesis.hashMerkleRoot = genesis.BuildMerkleTree();
        genesis.nVersion = 1;
        genesis.nTime = 1522479067;
        genesis.nBits = 0x1e0ffff0;
        genesis.nNonce = 21231226;

        hashGenesisBlock = genesis.GetHash();
        assert(hashGenesisBlock == uint256("0x000008d83ed83d3c080def52a2d534af4c663522b138ade392572ef0e7feb18e"));
        assert(genesis.hashMerkleRoot == uint256("0x90a64b2bbca84a0f4fe84514e83979023c35daacff69b15ab8d4e26c217b0191"));

        vSeeds.push_back(CDNSSeedData("alpha.elliotproject.org",    "alpha.elliotproject.org"));
        vSeeds.push_back(CDNSSeedData("bravo.elliotproject.org",    "bravo.elliotproject.org"));
        vSeeds.push_back(CDNSSeedData("charlie.elliotproject.org",  "charlie.elliotproject.org"));
        vSeeds.push_back(CDNSSeedData("delta.elliotproject.org",    "delta.elliotproject.org"));
        vSeeds.push_back(CDNSSeedData("echo.elliotproject.org",     "echo.elliotproject.org"));
        vSeeds.push_back(CDNSSeedData("foxtrot.elliotproject.org",  "foxtrot.elliotproject.org"));
        vSeeds.push_back(CDNSSeedData("golf.elliotproject.org",     "golf.elliotproject.org"));
        vSeeds.push_back(CDNSSeedData("hotel.elliotproject.org",    "hotel.elliotproject.org"));
        vSeeds.push_back(CDNSSeedData("india.elliotproject.org",    "india.elliotproject.org"));
        vSeeds.push_back(CDNSSeedData("juliet.elliotproject.org",   "juliet.elliotproject.org"));

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1, 33); // starts with E
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1, 13);
        base58Prefixes[SECRET_KEY] = std::vector<unsigned char>(1, 212);
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x02)(0x2D)(0x25)(0x33).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x02)(0x21)(0x31)(0x2B).convert_to_container<std::vector<unsigned char> >();
        // 	BIP44 coin type TODO
        base58Prefixes[EXT_COIN_TYPE] = boost::assign::list_of(0x80)(0x00)(0x00)(0x77).convert_to_container<std::vector<unsigned char> >();

        convertSeed6(vFixedSeeds, pnSeed6_main, ARRAYLEN(pnSeed6_main));

        fMiningRequiresPeers = true;
        fAllowMinDifficultyBlocks = false;
        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;
        fSkipProofOfWorkCheck = false;
        fTestnetToBeDeprecatedFieldRPC = false;
        fHeadersFirstSyncingActive = false;

        nPoolMaxTransactions = 3;
        strSporkKey = "04e5cd2be07aeb58fbc6f4bfbcf19dedb263a1bad4b3da794c751ed251ef12a30e73081ceeea4d440ef2929becf36fdc821b1942926ced251de5b244d91b67c6ba";
        strObfuscationPoolDummyAddress = "EQ37iidUyFaY3ozZRrQCth1zP3hhys52Py";
        nStartMasternodePayments = 1522479067;

        /** Zerocoin */
        zerocoinModulus = "25195908475657893494027183240048398571429282126204032027777137836043662020707595556264018525880784"
            "4069182906412495150821892985591491761845028084891200728449926873928072877767359714183472702618963750149718246911"
            "6507761337985909570009733045974880842840179742910064245869181719511874612151517265463228221686998754918242243363"
            "7259085141865462043576798423387184774447920739934236584823824281198163815010674810451660377306056201619676256133"
            "8441436038339044149526344321901146575444541784240209246165157233507787077498171257724679629263863563732899121548"
            "31438167899885040445364023527381951378636564391212010397122822120720357"; // RSA2048
        nMaxZerocoinSpendsPerTransaction = 7; // Assume about 20kb each
        nMinZerocoinMintFee = 1 * CENT; //high fee required for zerocoin mints
        nMintRequiredConfirmations = 20; //the maximum amount of confirmations until accumulated in 19
        nRequiredAccumulation = 1;
        nDefaultSecurityLevel = 100; //full security level for accumulators
        nZerocoinHeaderVersion = 4; //Block headers must be this version once zerocoin is active
        nBudget_Fee_Confirmations = 6; // Number of confirmations for the finalization fee
    }

    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        return data;
    }
};
static CMainParams mainParams;

/**
 * Testnet (v3)
 */
class CTestNetParams : public CMainParams
{
public:
    CTestNetParams()
    {
        networkID = CBaseChainParams::TESTNET;
        strNetworkID = "test";

        pchMessageStart[0] = 0x67;
        pchMessageStart[1] = 0x95;
        pchMessageStart[2] = 0x75;
        pchMessageStart[3] = 0x44;

        vAlertPubKey = ParseHex("047f34248d7717fee995f669ad25fa457ca1c6ef6052989083a25d3f47c2aa3f6cd815b7c4db3254d6a51a1a6dc4691c29f50e37355eb10bbec2fad2c2145366ce");
        nDefaultPort = 61319;

        //nEnforceBlockUpgradeMajority = 51;
        //nRejectBlockOutdatedMajority = 75;
        //nToCheckBlockUpgradeMajority = 100;

        nMinerThreads = 0;
        nTargetTimespan = 1 * 60; // ELLI: 1 day
        nTargetSpacing = 1 * 60;  // ELLI: 1 minute
        nLastPOWBlock = 200;
        nMaturity = 15;
        nMasternodeCountDrift = 4;
        nModifierUpdateBlock = 1; //approx Mon, 17 Apr 2017 04:00:00 GMT
        nMaxMoneyOut = 21000000 * COIN;
        nZerocoinStartHeight = nLastPOWBlock+10;
        nZerocoinStartTime = 1506779240+nZerocoinStartHeight*nTargetSpacing;
        nBlockEnforceSerialRange = 1; //Enforce serial range starting this block
        nBlockRecalculateAccumulators = nZerocoinStartHeight*2; //Trigger a recalculation of accumulators
        nBlockFirstFraudulent = nZerocoinStartHeight*2; //First block that bad serials emerged
        nBlockLastGoodCheckpoint = nZerocoinStartHeight*2; //Last valid accumulator checkpoint
        nBlockEnforceInvalidUTXO = nZerocoinStartHeight*2; //Start enforcing the invalid UTXO's
        nBlockEnforceMasternodePay = 1;
        //! Modify the testnet genesis block so the timestamp is valid for a later start.
        genesis.nTime = 1522479068;
        genesis.nNonce = 20800966;

        hashGenesisBlock = genesis.GetHash();
        assert(hashGenesisBlock == uint256("0x00000dd89ea8ecc3b47b1d60520120aa23835539b615f16ada62187fa763cb18"));

        vFixedSeeds.clear();
        vSeeds.clear();

        vSeeds.push_back(CDNSSeedData("talpha.elliotproject.org",    "talpha.elliotproject.org"));
        vSeeds.push_back(CDNSSeedData("tbravo.elliotproject.org",    "tbravo.elliotproject.org"));
        vSeeds.push_back(CDNSSeedData("tcharlie.elliotproject.org",  "tcharlie.elliotproject.org"));
        vSeeds.push_back(CDNSSeedData("tdelta.elliotproject.org",    "tdelta.elliotproject.org"));
        vSeeds.push_back(CDNSSeedData("techo.elliotproject.org",     "techo.elliotproject.org"));
        vSeeds.push_back(CDNSSeedData("tfoxtrot.elliotproject.org",  "tfoxtrot.elliotproject.org"));
        vSeeds.push_back(CDNSSeedData("tgolf.elliotproject.org",     "tgolf.elliotproject.org"));
        vSeeds.push_back(CDNSSeedData("thotel.elliotproject.org",    "thotel.elliotproject.org"));
        vSeeds.push_back(CDNSSeedData("tindia.elliotproject.org",    "tindia.elliotproject.org"));
        vSeeds.push_back(CDNSSeedData("tjuliet.elliotproject.org",   "tjuliet.elliotproject.org"));

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1, 139); // Testnet elli addresses start with 'x' or 'y'
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1, 19);  // Testnet elli script addresses start with '8' or '9'
        base58Prefixes[SECRET_KEY] = std::vector<unsigned char>(1, 239);     // Testnet private keys start with '9' or 'c' (Bitcoin defaults)
        // Testnet elli BIP32 pubkeys start with 'DRKV'
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x3a)(0x80)(0x61)(0xa0).convert_to_container<std::vector<unsigned char> >();
        // Testnet elli BIP32 prvkeys start with 'DRKP'
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x3a)(0x80)(0x58)(0x37).convert_to_container<std::vector<unsigned char> >();
        // Testnet elli BIP44 coin type is '1' (All coin's testnet default)
        base58Prefixes[EXT_COIN_TYPE] = boost::assign::list_of(0x80)(0x00)(0x00)(0x01).convert_to_container<std::vector<unsigned char> >();

        convertSeed6(vFixedSeeds, pnSeed6_test, ARRAYLEN(pnSeed6_test));

        fMiningRequiresPeers = true;
        fAllowMinDifficultyBlocks = true;
        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;
        fTestnetToBeDeprecatedFieldRPC = true;

        nPoolMaxTransactions = 2;
        strSporkKey = "04c30650c2f5681f3e487c89fa7e88576a064a6007145c4726769b79492c7c4c87b7852abf67fe0c952b9f81e45a422b23b4a9d0805e18d126473ae5499e84206a";
        strObfuscationPoolDummyAddress = "y57cqfGRkekRyDRNeJiLtYVEbvhXrNbmox";
        nStartMasternodePayments = 1522479068; 
        nBudget_Fee_Confirmations = 3; // Number of confirmations for the finalization fee. We have to make this very short
                                       // here because we only have a 8 block finalization window on testnet
    }
    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        return dataTestnet;
    }
};
static CTestNetParams testNetParams;

/**
 * Regression test
 */
class CRegTestParams : public CTestNetParams
{
public:
    CRegTestParams()
    {
        networkID = CBaseChainParams::REGTEST;
        strNetworkID = "regtest";
        strNetworkID = "regtest";

        pchMessageStart[0] = 0xee;
        pchMessageStart[1] = 0xa1;
        pchMessageStart[2] = 0x6c;
        pchMessageStart[3] = 0x6c;

        nSubsidyHalvingInterval = 150;

        //nEnforceBlockUpgradeMajority = 750;
        //nRejectBlockOutdatedMajority = 950;
        //nToCheckBlockUpgradeMajority = 1000;

        nMinerThreads = 1;
        nTargetTimespan = 24 * 60 * 60; // ELLI: 1 day
        nTargetSpacing = 1 * 60;        // ELLI: 1 minutes
        bnProofOfWorkLimit = ~uint256(0) >> 1;
        genesis.nTime = 1522479068;
        genesis.nBits = 0x207fffff;
        genesis.nNonce = 2;

        hashGenesisBlock = genesis.GetHash();
        nDefaultPort = 61321;
        assert(hashGenesisBlock == uint256("0x56c672f4a3339263e6a5882fa2123f57edfe0b9975d93bc0437ff05a930ba20b"));

        vFixedSeeds.clear(); //! Testnet mode doesn't have any fixed seeds.
        vSeeds.clear();      //! Testnet mode doesn't have any DNS seeds.

        fMiningRequiresPeers = false;
        fAllowMinDifficultyBlocks = true;
        fDefaultConsistencyChecks = true;
        fRequireStandard = false;
        fMineBlocksOnDemand = true;
        fTestnetToBeDeprecatedFieldRPC = false;
    }
    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        return dataRegtest;
    }
};
static CRegTestParams regTestParams;

/**
 * Unit test
 */
class CUnitTestParams : public CMainParams, public CModifiableParams
{
public:
    CUnitTestParams()
    {
        networkID = CBaseChainParams::UNITTEST;
        strNetworkID = "unittest";
        nDefaultPort = 61323;
        vFixedSeeds.clear(); //! Unit test mode doesn't have any fixed seeds.
        vSeeds.clear();      //! Unit test mode doesn't have any DNS seeds.

        fMiningRequiresPeers = false;
        fDefaultConsistencyChecks = true;
        fAllowMinDifficultyBlocks = false;
        fMineBlocksOnDemand = true;
    }

    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        // UnitTest share the same checkpoints as MAIN
        return data;
    }

    //! Published setters to allow changing values in unit test cases
    virtual void setSubsidyHalvingInterval(int anSubsidyHalvingInterval) { nSubsidyHalvingInterval = anSubsidyHalvingInterval; }
    virtual void setEnforceBlockUpgradeMajority(int anEnforceBlockUpgradeMajority) { nEnforceBlockUpgradeMajority = anEnforceBlockUpgradeMajority; }
    virtual void setRejectBlockOutdatedMajority(int anRejectBlockOutdatedMajority) { nRejectBlockOutdatedMajority = anRejectBlockOutdatedMajority; }
    virtual void setToCheckBlockUpgradeMajority(int anToCheckBlockUpgradeMajority) { nToCheckBlockUpgradeMajority = anToCheckBlockUpgradeMajority; }
    virtual void setDefaultConsistencyChecks(bool afDefaultConsistencyChecks) { fDefaultConsistencyChecks = afDefaultConsistencyChecks; }
    virtual void setAllowMinDifficultyBlocks(bool afAllowMinDifficultyBlocks) { fAllowMinDifficultyBlocks = afAllowMinDifficultyBlocks; }
    virtual void setSkipProofOfWorkCheck(bool afSkipProofOfWorkCheck) { fSkipProofOfWorkCheck = afSkipProofOfWorkCheck; }
};
static CUnitTestParams unitTestParams;


static CChainParams* pCurrentParams = 0;

CModifiableParams* ModifiableParams()
{
    assert(pCurrentParams);
    assert(pCurrentParams == &unitTestParams);
    return (CModifiableParams*)&unitTestParams;
}

const CChainParams& Params()
{
    assert(pCurrentParams);
    return *pCurrentParams;
}

CChainParams& Params(CBaseChainParams::Network network)
{
    switch (network) {
    case CBaseChainParams::MAIN:
        return mainParams;
    case CBaseChainParams::TESTNET:
        return testNetParams;
    case CBaseChainParams::REGTEST:
        return regTestParams;
    case CBaseChainParams::UNITTEST:
        return unitTestParams;
    default:
        assert(false && "Unimplemented network");
        return mainParams;
    }
}

void SelectParams(CBaseChainParams::Network network)
{
    SelectBaseParams(network);
    pCurrentParams = &Params(network);
}

bool SelectParamsFromCommandLine()
{
    CBaseChainParams::Network network = NetworkIdFromCommandLine();
    if (network == CBaseChainParams::MAX_NETWORK_TYPES)
        return false;

    SelectParams(network);
    return true;
}
