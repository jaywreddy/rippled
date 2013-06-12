#ifndef RIPPLE_TRANSACTIONACQUIRE_H
#define RIPPLE_TRANSACTIONACQUIRE_H

DEFINE_INSTANCE(TransactionAcquire);

// VFALCO TODO rename to PeerTxRequest
// A transaction set we are trying to acquire
class TransactionAcquire
    : private IS_INSTANCE (TransactionAcquire)
    , public PeerSet
    , public boost::enable_shared_from_this <TransactionAcquire>
{
public:
	typedef boost::shared_ptr<TransactionAcquire> pointer;

public:
	explicit TransactionAcquire (uint256 const& hash);
	virtual ~TransactionAcquire()		{ ; }

	SHAMap::ref getMap()				{ return mMap; }

	SHAMapAddNode takeNodes(const std::list<SHAMapNode>& IDs,
		const std::list< Blob >& data, Peer::ref);

private:
	SHAMap::pointer		mMap;
	bool				mHaveRoot;

	void onTimer(bool progress);
	void newPeer(Peer::ref peer)	{ trigger(peer); }

	void done();
	void trigger(Peer::ref);
	boost::weak_ptr<PeerSet> pmDowncast();
};

#endif