#define BOOST_TEST_MODULE PlayersTest
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#include <memory>
#include "state.h"

using namespace ::state;

// ---------- Player (testé à travers Detective) ----------

BOOST_AUTO_TEST_CASE(TestPlayerInitialState)
{
  Detective d("rouge");
  BOOST_CHECK_EQUAL(d.getColor(), "rouge");
  BOOST_CHECK_EQUAL(d.getCurrentStationId(), 0);
}

BOOST_AUTO_TEST_CASE(TestPlayerSetters)
{
  Detective d("rouge");
  d.setColor("bleu");
  d.setCurrent_station_id(13);
  BOOST_CHECK_EQUAL(d.getColor(), "bleu");
  BOOST_CHECK_EQUAL(d.getCurrent_station_id(), 13);
  BOOST_CHECK_EQUAL(d.getCurrentStationId(), 13);
}

BOOST_AUTO_TEST_CASE(TestPlayerMakeMove)
{
  Detective d("rouge");
  BOOST_CHECK(d.makeMove(14, TaxiTicket));
  BOOST_CHECK_EQUAL(d.getCurrentStationId(), 14);
}

// ---------- Detective ----------

BOOST_AUTO_TEST_CASE(TestDetectiveStartTickets)
{
  Detective d("rouge");
  BOOST_CHECK(d.hasTicket(TaxiTicket));
  BOOST_CHECK(d.hasTicket(BusTicket));
  BOOST_CHECK(d.hasTicket(TrainTicket));
  BOOST_CHECK(!d.hasTicket(BlackTicket));   // un détective n'a pas de Black Ticket
}

BOOST_AUTO_TEST_CASE(TestDetectiveUsesAllTrainTickets)
{
  Detective d("rouge");
  for (int i = 0; i < 4; i++) {             // 4 tickets métro au départ
    BOOST_CHECK(d.hasTicket(TrainTicket));
    d.useTransportTicket(TrainTicket);
  }
  BOOST_CHECK(!d.hasTicket(TrainTicket));   // plus aucun
  BOOST_CHECK(d.hasTicket(TaxiTicket));     // les autres ne sont pas touchés
}

BOOST_AUTO_TEST_CASE(TestDetectiveTicketsNeverNegative)
{
  Detective d("rouge");
  for (int i = 0; i < 10; i++) d.useTransportTicket(TrainTicket);  // plus que 4
  BOOST_CHECK(!d.hasTicket(TrainTicket));
}

// ---------- MrX ----------

BOOST_AUTO_TEST_CASE(TestMrXStartTickets)
{
  MrX x("noir");
  BOOST_CHECK(x.hasBlackTicket());
  BOOST_CHECK(x.hasDoubleTicket());
}

BOOST_AUTO_TEST_CASE(TestMrXBlackTickets)
{
  MrX x("noir");
  for (int i = 0; i < 5; i++) {             // 5 Black Tickets au départ
    BOOST_CHECK(x.hasBlackTicket());
    x.useTransportTicket(BlackTicket);
  }
  BOOST_CHECK(!x.hasBlackTicket());
}

BOOST_AUTO_TEST_CASE(TestMrXNormalTicketsDontUseBlack)
{
  MrX x("noir");
  for (int i = 0; i < 10; i++) x.useTransportTicket(TaxiTicket);  // pioche commune
  BOOST_CHECK(x.hasBlackTicket());          // aucun Black Ticket consommé
}

BOOST_AUTO_TEST_CASE(TestMrXDoubleTickets)
{
  MrX x("noir");
  x.use_double_ticket();
  BOOST_CHECK(x.hasDoubleTicket());         // il en reste 1
  x.use_double_ticket();
  BOOST_CHECK(!x.hasDoubleTicket());        // plus aucun
  x.use_double_ticket();                    // ne doit pas planter ni passer en négatif
  BOOST_CHECK(!x.hasDoubleTicket());
}

// ---------- Polymorphisme (le "virtual") ----------

BOOST_AUTO_TEST_CASE(TestPolymorphismThroughPlayerPointer)
{
  std::unique_ptr<Player> p = std::make_unique<MrX>("noir");
  for (int i = 0; i < 5; i++) p->useTransportTicket(BlackTicket);   // appelle la version de MrX

  MrX* x = dynamic_cast<MrX*>(p.get());
  BOOST_REQUIRE(x != nullptr);
  BOOST_CHECK(!x->hasBlackTicket());        // prouve que MrX::useTransportTicket a été appelé
}